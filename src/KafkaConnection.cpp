
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file KafkaConnection.cpp
 *  This file provides a template for implementing a middleware wrapper.
 */

#include <KafkaConnection.h>

#include <gmsec/internal/rawbuf.h>


using namespace gmsec;
using namespace gmsec::util;

using namespace std;


KafkaConnection::KafkaConnection(Config *cfg)
		:
		BaseConnection(cfg)
{
	Status result;

	char raw[4000];
	rawbuf buffer(raw, sizeof(raw));
	ostream os(&buffer);

	os << "KafkaConnection::KafkaConnection:";

	if (NULL != cfg)
	{
		const char * key;
		const char * value;
		for (result = cfg->GetFirst(key, value);
		        GMSEC_STATUS_NO_ERROR != result.GetCode();
		        result = cfg->GetNext(key, value))
		{
			os << "\n\t(" << key << ',' << value << ')';
		}
	}
	os << ends;

	LOG_DEBUG << raw;
}


KafkaConnection::~KafkaConnection()
{
	LOG_DEBUG << "gmsec_kafka:KafkaConnection::~KafkaConnection()";
}


const char *KafkaConnection::GetLibraryVersion()
{
	return "v0.2.1";
}


Status KafkaConnection::mwConnect()
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwConnect()";

	return result;
}


Status KafkaConnection::mwDisconnect()
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwDisconnect()";

	return result;
}


Status KafkaConnection::mwSubscribe(const char *subject, const gmsec::Config &config)
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwSubscribe(" << subject << ')';

	return result;
}


Status KafkaConnection::mwUnSubscribe(const char *subject)
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwUnSubscribe(" << subject << ')';

	return result;
}


Status KafkaConnection::mwCreateMessage(const char *subject,
		GMSEC_MSG_KIND kind, Message *&msg)
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwCreateMessage("
			<< (subject ? subject : "[null]")
			<< ", kind=" << kind << ", msg=" << msg << ')';

	{
		KafkaMessage * tmp = new KafkaMessage();
		tmp->SetSubject(subject);
		tmp->SetKind(kind);
		msg = tmp->createExternal();
	}

	if (!msg)
	{
		result.Set(
		    GMSEC_STATUS_MESSAGE_ERROR,
		    GMSEC_INVALID_MESSAGE,
		    "KafkaMessage object not created");
		LOG_WARNING << result.Get();
	}

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwCreateMessage => " << msg;

	return result;
}


#if 0
Status KafkaConnection::DestroyMessage(Message *msg)
{
	Status result;

	LOG_INFO << "gmsec_kafka:KafkaConnection::DestroyMessage(" << msg << ')';

	return result;
}
#endif


Status KafkaConnection::mwPublish(Message *msg, const gmsec::Config &config)
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::Publish(" << msg << ')';

	return result;
}


Status KafkaConnection::mwRequest(Message *request, std::string &id)
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwRequest(" << request << ')';

	return result;
}


Status KafkaConnection::mwReply(Message *request, Message *reply)
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwReply(request=" << request
			<< ", reply=" << reply << ')';

	return result;
}


Status KafkaConnection::mwGetNextMsg(Message *&msg, GMSEC_I32 timeout)
{
	Status result;

	LOG_DEBUG << "gmsec_kafka:KafkaConnection::mwGetNextMsg(msg=" << msg
			<< ", timeout=" << timeout << ')';

	msg = NULL;
	result.Set(GMSEC_STATUS_CONNECTION_ERROR,
			GMSEC_NO_MESSAGE_AVAILABLE,
			"Nothing left.");

	return result;
}


//	EOF KafkaConnection.cpp
