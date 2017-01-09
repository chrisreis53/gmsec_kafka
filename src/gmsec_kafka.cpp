
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file gmsec_kafka.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#include <gmsec_kafka.h>

#include <gmsec/internal/Log.h>
#include <gmsec/internal/StatusException.h>

#include <KafkaConnection.h>


using namespace gmsec;
using namespace gmsec::util;


GMSEC_KAFKA_API Status *CreateConnection(Config *cfg, Connection **conn)
{
	static Status result;

	result.ReSet();

	LOG_DEBUG << "gmsec_kafka:CreateConnection(" << cfg << ", " << conn << ")";

	try
	{
		KafkaConnection * tmp = new KafkaConnection(cfg);
		*conn = tmp->createExternal();
	}
	catch (gmsec::internal::StatusException &se)
	{
		result = se;
	}

	LOG_DEBUG << "gmsec_kafka: *conn=" << *conn;

	return &result;
}


GMSEC_KAFKA_API void DestroyConnection(Connection *conn)
{
	LOG_DEBUG << "gmsec_kafka:DestroyConnection(" << conn << ')';

	if (conn)
		gmsec::internal::ConnectionBuddy::destroy(conn);
}


//	EOF	gmsec_kafka.cpp
