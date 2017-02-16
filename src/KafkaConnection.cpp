
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file KafkaConnection.cpp
 *  This file provides a template for implementing a middleware wrapper.
 */
#include <gmsec_kafka.h>

#include <KafkaConnection.h>

#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/Rawbuf.h>

#include <gmsec4/Connection.h>
#include <gmsec4/Errors.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/Condition.h>
#include <gmsec4/util/Log.h>
#include <gmsec4/util/Mutex.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <regex>

#include <gmsec_version.h>

#include <librdkafka/rdkafkacpp.h>

using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;

using namespace std;

static bool run = true;
static bool exit_eof = false;
int use_ccb = 0;

//Kafka base objects
RdKafka::Producer *producer;
RdKafka::Consumer *consumer;
RdKafka::Queue *rkqu;
//Kafka vector objects

static void sigterm (int sig) {
  run = false;
}

void msg_consume(RdKafka::Message* message, void* opaque) {
  switch (message->err()) {
    case RdKafka::ERR__TIMED_OUT:
      break;

    case RdKafka::ERR_NO_ERROR:
      /* Real message */
      std::cout << "Read msg at offset " << message->offset() << std::endl;
      if (message->key()) {
        std::cout << "Key: " << *message->key() << std::endl;
      }

      printf("%.*s\n",
        static_cast<int>(message->len()),
        static_cast<const char *>(message->payload()));
      break;

    case RdKafka::ERR__PARTITION_EOF:
      /* Last message */
      if (exit_eof) {
        run = false;
      }
      break;

    case RdKafka::ERR__UNKNOWN_TOPIC:
    case RdKafka::ERR__UNKNOWN_PARTITION:
      std::cerr << "Consume failed: " << message->errstr() << std::endl;
      run = false;
      break;

    default:
      /* Errors */
      std::cerr << "Consume failed: " << message->errstr() << std::endl;
      run = false;
  }
}

std::vector<std::string> list_topics(std::string input_topic){
  std::vector<std::string> str_vec;
  RdKafka::Topic *topic = NULL;
  class RdKafka::Metadata *metadata;

  /* Fetch metadata */
  RdKafka::ErrorCode err = producer->metadata(topic!=NULL, topic, &metadata, 5000);
  if (err != RdKafka::ERR_NO_ERROR) {
    std::cerr << "%% Failed to acquire metadata: " << RdKafka::err2str(err) << std::endl;
  }

  std::cout << metadata->topics()->size() << " topics:" << std::endl;
  RdKafka::Metadata::TopicMetadataIterator it;
  for (it = metadata->topics()->begin(); it != metadata->topics()->end(); ++it) {
    std::string t = (*it)->topic();
    std::regex re(input_topic);
    if (regex_match(t,re)) {
      str_vec.push_back(t);
      std::cout << "Found: " << t << '\n';
    }
  }

  return str_vec;
}

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
 public:
  void dr_cb (RdKafka::Message &message) {
    GMSEC_INFO << "TEST" << '\n';
    std::cout << "Message delivery for (" << message.len() << " bytes): " <<
        message.errstr() << '\n';
    if (message.key())
      std::cout << "Key: " << *(message.key()) << ";" << '\n';
  }
};

class ExampleConsumeCb : public RdKafka::ConsumeCb {
 public:
  void consume_cb (RdKafka::Message &msg, void *opaque) {
    msg_consume(&msg, opaque);
  }
};

KafkaConnection::KafkaConnection(const Config& config)
	:
	mw_test(false),
	mwInfo(""),
  mw_brokers("localhost"),
  mw_errstr(""),
  mw_debug("")
{
	GMSEC_DEBUG << "Connection test" << '\n';

  GMSEC_INFO << '\n' << config.toXML() << '\n';
  if(config.getValue("server") != NULL){
    mw_brokers = config.getValue("server");
  }
  GMSEC_DEBUG << "Brokers: " << mw_brokers.c_str() << '\n';

  /*
   * Create producer using accumulated global configuration.
   */
  RdKafka::Conf *pub_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  pub_conf->set("metadata.broker.list", mw_brokers, mw_errstr);
  ExampleDeliveryReportCb ex_dr_cb;

  //TODO Fix callback issues
  //conf->set("dr_cb", &ex_dr_cb, mw_errstr);
  producer = RdKafka::Producer::create(pub_conf, mw_errstr);
  if (!producer) {
    std::cerr << "Failed to create producer: " << mw_errstr << std::endl;
    exit(1);
  }

  std::cout << "% Created producer " << producer->name() << std::endl;

  RdKafka::Conf *sub_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  sub_conf->set("metadata.broker.list", mw_brokers, mw_errstr);

  consumer = RdKafka::Consumer::create(sub_conf, mw_errstr);
  if (!consumer) {
    std::cerr << "Failed to create consumer: " << mw_errstr << std::endl;
    exit(1);
  }
  std::cout << "% Created consumer " << consumer->name() << std::endl;

  rkqu = RdKafka::Queue::create(consumer);

}

KafkaConnection::~KafkaConnection()
{
	GMSEC_DEBUG << "~Connection" << '\n';
}

const char* KafkaConnection::getLibraryVersion()
{
	return "v0.10.1";
}

const char* KafkaConnection::getMWInfo()
{
	if (mwInfo.empty())
	{
		mwInfo = getLibraryRootName();
	}

	return mwInfo.c_str();
}

void KafkaConnection::mwConnect()
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwConnect()";
}

void KafkaConnection::mwDisconnect()
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwDisconnect()";
}

void KafkaConnection::mwSubscribe(const char* subject, const Config& config)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwSubscribe(" << subject << ')' << '\n';

  std::string topic_str = subject;
  std::vector<std::string> topic_vec;

  if (topic_str.find("*") != string::npos) {
    GMSEC_DEBUG << "Found Wildcard in " << topic_str.c_str() << '\n';
    topic_vec = list_topics(topic_str);
    for (std::vector<std::string>::const_iterator i = topic_vec.begin(); i != topic_vec.end(); ++i) {
      std::string it_str = *i;
      GMSEC_DEBUG << "Subscribing to: " << it_str.c_str() << '\n';
      RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
      RdKafka::Conf *sub_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
      sub_conf->set("metadata.broker.list", mw_brokers, mw_errstr);
      RdKafka::Consumer *t_consumer = RdKafka::Consumer::create(sub_conf, mw_errstr);
      /*
       * Create topic handle.
       */
      RdKafka::Topic *topic = RdKafka::Topic::create(consumer, it_str, tconf, mw_errstr);
      if (!topic) {
        std::cerr << "Failed to create topic: " << mw_errstr << std::endl;
        exit(1);
      }
      /*
       * Start consumer for topic+partition at start offset
       */
      int32_t partition = 0;
      int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;
      RdKafka::ErrorCode resp = consumer->start(topic, partition, start_offset, rkqu);
      if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Failed to start consumer: " <<
        RdKafka::err2str(resp) << std::endl;
        exit(1);
      }
    }
  } else {
    GMSEC_DEBUG << "No Wildcard" << '\n';
    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    RdKafka::Conf *sub_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    sub_conf->set("metadata.broker.list", mw_brokers, mw_errstr);
    RdKafka::Consumer *t_consumer = RdKafka::Consumer::create(sub_conf, mw_errstr);
    /*
     * Create topic handle.
     */
    RdKafka::Topic *topic = RdKafka::Topic::create(t_consumer, topic_str, tconf, mw_errstr);
    if (!topic) {
      std::cerr << "Failed to create topic: " << mw_errstr << std::endl;
      exit(1);
    }
    /*
     * Start consumer for topic+partition at start offset
     */
    int32_t partition = 0;
    int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;
    RdKafka::ErrorCode resp = t_consumer->start(topic, partition, start_offset, rkqu);
    if (resp != RdKafka::ERR_NO_ERROR) {
      std::cerr << "Failed to start consumer: " <<
      RdKafka::err2str(resp) << std::endl;
      exit(1);
    }


  }

}

void KafkaConnection::mwUnsubscribe(const char *subject)
{
  int32_t partition = 0;
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwUnSubscribe(" << subject << ')';
  RdKafka::Topic *topic = RdKafka::Topic::create(consumer, subject, tconf, mw_errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << mw_errstr << std::endl;
    exit(1);
  }
  consumer->stop(topic, partition);
}

void KafkaConnection::mwPublish(const Message& message, const Config& config)
{
  Message test_message(message);
  std::string topic_str = message.getSubject();
  std::string line = message.toXML();
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  /*
   * Create topic handle.
   */
  RdKafka::Topic *topic = RdKafka::Topic::create(producer, topic_str,
             tconf, mw_errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << mw_errstr << std::endl;
    exit(1);
  }
  /*
   * Publish Message
   */
   int32_t partition = RdKafka::Topic::PARTITION_UA;
   RdKafka::ErrorCode resp = producer->produce(topic, partition, RdKafka::Producer::RK_MSG_COPY, const_cast<char *>(line.c_str()), line.size(), NULL, NULL);
   if (resp != RdKafka::ERR_NO_ERROR){
     std::cerr << "% Produce failed: " <<
     RdKafka::err2str(resp) << std::endl;
   }else{
     std::cerr << "% Produced message (" << line.size() << " bytes)" << std::endl;
     producer->poll(0);
   }

  //  while (run && producer->outq_len() > 0) {
  //    std::cerr << "Waiting for " << producer->outq_len() << std::endl;
  //    producer->poll(250);
  //  }

	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::Publish(things)" << '\n' << message.toXML() ;
}

void KafkaConnection::mwRequest(const Message& message, std::string& id)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwRequest(" << ')';
}

void KafkaConnection::mwReply(const Message& request, const Message& reply)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwReply(request=" << ", reply=" << ')';
}

void KafkaConnection::mwReceive(Message*& message, GMSEC_I32 timeout)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwReceive";

  message = NULL;

  RdKafka::Message *msg = consumer->consume(rkqu, (int)timeout);
  std::string msg_str = static_cast<const char *>(msg->payload());

  if(msg_str.find("Broker:") != string::npos) {
    GMSEC_INFO << msg_str.c_str();
  } else {
    GMSEC_INFO << '\n' << msg_str.c_str();
    message = new Message(msg_str.c_str());
  }

  //msg_consume(msg, NULL);
  delete msg;

}

//	EOF KafkaConnection.cpp
