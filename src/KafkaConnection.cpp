
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


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;

using namespace std;

static bool run = true;
static bool exit_eof = false;

static void sigterm (int sig) {
  run = false;
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

KafkaConnection::KafkaConnection(const Config& config)
	:
	mw_test(false),
	mwInfo("")
{
	GMSEC_DEBUG << "Connection test" << '\n';

  std::string brokers = "localhost";
  std::string errstr;
  std::string topic_str;
  std::string mode;
  std::string debug;


  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  ExampleDeliveryReportCb ex_dr_cb;

  conf->set("metadata.broker.list", brokers, errstr);
  conf->set("dr_cb", &ex_dr_cb, errstr);

  /*
   * Create producer using accumulated global configuration.
   */
  RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
  if (!producer) {
    std::cerr << "Failed to create producer: " << errstr << std::endl;
    exit(1);
  }

  std::cout << "% Created producer " << producer->name() << std::endl;

  /*
   * Create topic handle.
   */
  RdKafka::Topic *topic = RdKafka::Topic::create(producer, topic_str,
             tconf, errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << errstr << std::endl;
    exit(1);
  }
  /*
   * Publish Message
   */
   //RdKafka::ErrorCode resp = producer->produce(topic, partition,

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
}

void KafkaConnection::mwUnsubscribe(const char *subject)
{
	GMSEC_DEBUG << "gmsec_kafka:KafkaConnection::mwUnSubscribe(" << subject << ')';
}

void KafkaConnection::mwPublish(const Message& message, const Config& config)
{
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
