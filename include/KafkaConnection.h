
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file KafkaConnection.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#ifndef _KafkaConnection_h_
#define _KafkaConnection_h_

#include <gmsec_kafka.h>

#include <gmsec4_defs.h>

#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/UniqueFilter.h>

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>


//#include <KafkaMessage.h>


/** @class VoidConnection
 * This class provides a sample (dummy) implementation of the
 * BaseConnection abstract class.
*/
class GMSEC_KAFKA_API KafkaConnection :	public gmsec::api::internal::ConnectionInterface
{
private:
	bool mw_test;

public:

	KafkaConnection(const gmsec::api::Config& config);

	virtual ~KafkaConnection();

	virtual const char* CALL_TYPE getLibraryVersion();

	virtual const char* CALL_TYPE getLibraryRootName()
	{
		return "gmsec_kafka";
	}

	virtual const char* getMWInfo();

	virtual void CALL_TYPE mwConnect();

	virtual void CALL_TYPE mwDisconnect();

	virtual void CALL_TYPE mwSubscribe(const char* subject, const gmsec::api::Config& config);

	virtual void CALL_TYPE mwUnsubscribe(const char* subject);

	virtual void CALL_TYPE mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config);

	virtual void CALL_TYPE mwRequest(const gmsec::api::Message& request, std::string& id);

	virtual void CALL_TYPE mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply);

	virtual void CALL_TYPE mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout);

};
#endif	//	VoidConnection_h
