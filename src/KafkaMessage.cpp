
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file KafkaMessage.cpp
 *  This file provides a template for implementing a middleware wrapper.
 */

#include <KafkaMessage.h>

#include <gmsec/internal/Log.h>


using namespace gmsec;
using namespace gmsec::util;

KafkaMessage::KafkaMessage()
{
	LOG_VERBOSE << "KafkaMessage::KafkaMessage()";
}


KafkaMessage::~KafkaMessage()
{
	LOG_VERBOSE << "KafkaMessage::~KafkaMessage()";
}


#ifdef SUBCLASS_BASEMESSAGE

bool KafkaMessage::isValid()
{
	LOG_VERBOSE << "KafkaMessage::isValid()";

	return (true);
}


Status KafkaMessage::SetKind(GMSEC_MSG_KIND kind)
{
	Status result;

	LOG_VERBOSE << "KafkaMessage::mwSetKind(" << kind << ')';

	mType = (size_t) kind;
	switch (kind)
	{
	case GMSEC_MSG_UNSET:
		printf("gmsec_kafka		Not Specified\n");
		break;
	case GMSEC_MSG_PUBLISH:
		printf("gmsec_kafka		Proxy - non-blocking\n");
		break;
	case GMSEC_MSG_REQUEST:
		printf("gmsec_kafka		Send()/Receive() - blocking\n");
		break;
	case GMSEC_MSG_REPLY:
		printf("gmsec_kafka		Reply() - non/un-blocking\n");
		break;
	default:
		printf("gmsec_kafka		Error\n");
		result.Set(
		    GMSEC_STATUS_MESSAGE_ERROR,
		    GMSEC_UNKNOWN_MSG_TYPE,
		    "Neither Send, Reply, nor Proxy message type");
		break;
	}

	return result;
}


Status KafkaMessage::GetKind(GMSEC_MSG_KIND &kind)
{
	Status result;

	LOG_DEBUG << "KafkaMessage::GetKind(" << &kind << ')';

	kind = mType;

	return result;
}


Status KafkaMessage::mwSetSubject(const char *subject)
{
	Status result;

	LOG_DEBUG << "KafkaMessage::mwSetSubject("
	       (subject ? subject : "[null]") << ')';

	return result;
}


Status KafkaMessage::GetSubject(const char *&subject)
{
	Status result;

	LOG_DEBUG << "KafkaConnection::GetSubject(" << &subject << ')';

	subject = mName;

	return result;
}


bool KafkaMessage::ProcessConfigValue(const char *name, const char *value)
{
	LOG_DEBUG << "KafkaConnection::ProcessConfigValue(name="
			<< (name ? name : "[null]")
			<< ", value=" << (value ? value : "[null]");

	return true;
}


Status KafkaMessage::ClearFields()
{
	Status result;

	LOG_DEBUG << "KafkaConnection::ClearFields()";

	return result;
}


Status KafkaMessage::ClearField(const char *name)
{
	Status result;

	LOG_DEBUG << "KafkaConnection::ClearField(name="
			<< (name ? name : "[null]") << ')';

	return result;
}


Status KafkaMessage::GetField(const char *name, Field &field)
{
	Status result;

	LOG_DEBUG << "KafkaConnection::GetField(name="
			<< (name ? name : "[null]")
			<< ", field=" << &field << ')';

	return result;
}


Status KafkaMessage::GetFieldCount(GMSEC_I32 &count)
{
	Status result;

	LOG_DEBUG << "KafkaConnection::GetFieldCount(@count=" << &count << ')';

	return result;
}


Status KafkaMessage::GetFirstField(Field &field)
{
	Status result;

	LOG_DEBUG << "KafkaConnection::GetFirstField(@field=" << &field << ')';

	return result;
}


Status KafkaMessage::GetNextField(Field &field)
{
	Status result;

	LOG_DEBUG << "KafkaConnection::GetNextField(@field=" << &field << ')';
	result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		GMSEC_FIELDS_END_REACHED,
		"Nominal end-of-fields");

	return result;
}


Status KafkaMessage::GetMSGSize(unsigned long &size)
{
	Status result;

	LOG_DEBUG << "KafkaConnection::GetMSGSize(@size=" << &size << ')';

	return result;
}

#endif /* SUBCLASS_BASEMESSAGE */


//	EOF	KafkaMessage.cpp
