
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

#include <gmsec_version.h>

#include "rdkafkacpp.h"
//#include <librdkafka/rdkafkacpp.h>

using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;

using namespace std;

static bool run = true;
static bool exit_eof = false;
int use_ccb = 0;

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

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
 public:
  void dr_cb (RdKafka::Message &message) {
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

  std::string topic_str;

  topic_str = "GMSEC.CONN.PUBLISH";

  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  ExampleDeliveryReportCb ex_dr_cb;

  conf->set("metadata.broker.list", mw_brokers, mw_errstr);
  conf->set("dr_cb", &ex_dr_cb, mw_errstr);

  /*
   * Create producer using accumulated global configuration.
   */
  RdKafka::Producer *producer = RdKafka::Producer::create(conf, mw_errstr);
  if (!producer) {
    std::cerr << "Failed to create producer: " << mw_errstr << std::endl;
    exit(1);
  }

  std::cout << "% Created producer " << producer->name() << std::endl;

  /*
   * Create topic handle.
   */
  RdKafka::Topic *topic = RdKafka::Topic::create(producer, topic_str,
             tconf, mw_errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << mw_errstr << std::endl;
    exit(1);
  }

  std::string line = "GMSEC Pub has Connected";
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

   while (run && producer->outq_len() > 0) {
     std::cerr << "Waiting for " << producer->outq_len() << std::endl;
     producer->poll(1000);
   }

   delete topic;
   delete producer;
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
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwSubscribe(" << subject << ')';

  std::string topic_str = subject;

  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  ExampleDeliveryReportCb ex_dr_cb;

  conf->set("metadata.broker.list", mw_brokers, mw_errstr);
  conf->set("dr_cb", &ex_dr_cb, mw_errstr);

  RdKafka::Consumer *consumer = RdKafka::Consumer::create(conf, mw_errstr);
  if (!consumer) {
    std::cerr << "Failed to create consumer: " << mw_errstr << std::endl;
    exit(1);
  }

  std::cout << "% Created consumer " << consumer->name() << std::endl;

  /*
   * Create topic handle.
   */
  RdKafka::Topic *topic = RdKafka::Topic::create(consumer, topic_str, tconf, mw_errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << mw_errstr << std::endl;
    exit(1);
  }

  /*
   * Start consumer for topic+partition at start offset
   */
  int32_t partition = 0;
  int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;
  RdKafka::ErrorCode resp = consumer->start(topic, partition, start_offset);
  if (resp != RdKafka::ERR_NO_ERROR) {
    std::cerr << "Failed to start consumer: " <<
    RdKafka::err2str(resp) << std::endl;
    exit(1);
  }

  ExampleConsumeCb ex_consume_cb;

  /*
   * Consume messages
   */
  while (run) {
    if (use_ccb) {
      consumer->consume_callback(topic, partition, 1000,
                                 &ex_consume_cb, &use_ccb);
    } else {
      RdKafka::Message *msg = consumer->consume(topic, partition, 1000);
      msg_consume(msg, NULL);
      delete msg;
    }
    consumer->poll(0);
  }

  /*
   * Stop consumer
   */
  consumer->stop(topic, partition);

  consumer->poll(1000);

  delete topic;
  delete consumer;
}

void KafkaConnection::mwUnsubscribe(const char *subject)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwUnSubscribe(" << subject << ')';
}

void KafkaConnection::mwPublish(const Message& message, const Config& config)
{
  std::string topic_str;

  topic_str = message.getSubject();

  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  ExampleDeliveryReportCb ex_dr_cb;

  conf->set("metadata.broker.list", mw_brokers, mw_errstr);
  conf->set("dr_cb", &ex_dr_cb, mw_errstr);

  /*
   * Create producer using accumulated global configuration.
   */
  RdKafka::Producer *producer = RdKafka::Producer::create(conf, mw_errstr);
  if (!producer) {
    std::cerr << "Failed to create producer: " << mw_errstr << std::endl;
    exit(1);
  }

  std::cout << "% Created producer " << producer->name() << std::endl;

  /*
   * Create topic handle.
   */
  RdKafka::Topic *topic = RdKafka::Topic::create(producer, topic_str,
             tconf, mw_errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << mw_errstr << std::endl;
    exit(1);
  }

  std::string line = message.toXML();
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

   while (run && producer->outq_len() > 0) {
     std::cerr << "Waiting for " << producer->outq_len() << std::endl;
     producer->poll(1000);
   }

   delete topic;
   delete producer;
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::Publish(things)" << message.toXML() ;
}

void KafkaConnection::mwRequest(const Message& message, std::string& id)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwRequest("<< ')';
}

void KafkaConnection::mwReply(const Message& request, const Message& reply)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwReply(request=" << ", reply=" << ')';
}

void KafkaConnection::mwReceive(Message*& message, GMSEC_I32 timeout)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwReceive";
}

//	EOF KafkaConnection.cpp
