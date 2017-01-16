
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

#include <gmsec_version.h>

#include <rdkafka.h>

using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;

using namespace std;

/**
 * Message delivery report callback.
 * Called once for each message.
 * See rdkafka.h for more information.
 */
// static void msg_delivered (rd_kafka_t *rk,
// 			   void *payload, size_t len,
// 			   int error_code,
// 			   void *opaque, void *msg_opaque) {
//
// 	if (error_code)
// 		fprintf(stderr, "%% Message delivery failed: %s\n",
// 			rd_kafka_err2str(error_code));
// 	else if (!quiet)
// 		fprintf(stderr, "%% Message delivered (%zd bytes): %.*s\n", len,
// 			(int)len, (const char *)payload);
// }

KafkaConnection::KafkaConnection(const Config& config)
	:
	mw_test(false),
	mwInfo("")
{
	GMSEC_DEBUG << "Connection" << '\n';

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
