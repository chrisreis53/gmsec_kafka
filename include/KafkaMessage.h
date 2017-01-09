
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file KafkaMessage.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#ifndef _KafkaMessage_h_
#define _KafkaMessage_h_

#include <gmsec_kafka.h>

#include <gmsec/internal/SimpleMessage.h>


/** @class KafkaMessage
* This class implements the Message abstract base class to support
* Kafka middleware.
*/
class GMSEC_KAFKA_API KafkaMessage
		:	public gmsec::internal::SimpleMessage
{
public:
	/** @fn KafkaMessage(subject, type)
	* Standard constructor that takes a kafka message pointer
	*/
	KafkaMessage();
	virtual ~KafkaMessage();

	/** @fn getCMessagePtr()
	* Access to the unlying kafka native message pointer.
	*/
	//	CMessage * CALL_TYPE getCMessagePtr() { return NULL; }

	/** @fn GetLibraryRootName()
	* Retrieve the root library name.
	*/
	virtual const char * CALL_TYPE GetLibraryRootName()
	{
		return "gmsec_kafka";
	}

#ifdef SUBCLASS_BASEMESSAGE

	/** @fn ProcessConfigValue(const char *name, const char *value)
	* Support function to add configuration value support for
	* this particular middleware.
	*/
	virtual bool CALL_TYPE ProcessConfigValue(const char *name, const char *value);

	/** @fn isValid()
	* Check to see if this message has a valid native pointer
	*/
	virtual bool CALL_TYPE isValid();

	/** @fn SetKind(GMSEC_MSG_KIND kind)
	* Set the message kind (PUBLISH,REQUEST,REPLY)
	*/
	virtual gmsec::Status CALL_TYPE mwSetKind(GMSEC_MSG_KIND kind);

	/** @fn GetKind(GMSEC_MSG_KIND &kind)
	* Get the message kind (PUBLISH,REQUEST,REPLY)
	*/
	virtual gmsec::Status CALL_TYPE GetKind(GMSEC_MSG_KIND &kind);

	/** @fn SetSubject(const char *subject)
	* Set the message subject
	*/
	virtual gmsec::Status CALL_TYPE mwSetSubject(const char *subject);

	/** @fn GetSubject(const char *&subject)
	* Get the message subject
	*/
	virtual gmsec::Status CALL_TYPE GetSubject(const char *&subject);

	/** @fn ClearFields()
	* Clear all fields.
	*/
	virtual gmsec::Status CALL_TYPE ClearFields();

	/** @fn AddField(gmsec::Field &field)
	* Add a field to this message. This will overwrite an existing field
	* of the same name.
	*/
	virtual gmsec::Status CALL_TYPE AddField(gmsec::Field &field);

	/** @fn ClearField(const char *name)
	* Clear this named field.
	*/
	virtual gmsec::Status CALL_TYPE ClearField(const char *name);

	/** @fn GetField(const char *name, gmsec::Field &field)
	* Get a field by name if available.
	*/
	virtual gmsec::Status CALL_TYPE GetField(const char *name, gmsec::Field &field);

	/** @fn GetFieldCount(long &count)
	* Get the number of available fields
	*/
	virtual gmsec::Status CALL_TYPE GetFieldCount(long &count);

	/** @fn GetMSGSize(unsigned long &size)
	* Get the physical message size
	*/
	virtual gmsec::Status CALL_TYPE GetMSGSize(unsigned long &size);

	/** @fn GetFirstField(gmsec::Field &field)
	* Iteration of fields, this will get the first field and reset the
	* internal iteration to the first field.
	*/
	virtual gmsec::Status CALL_TYPE GetFirstField(gmsec::Field &field);

	/** @fn GetNextField(gmsec::Field &field)
	* Iteration of fields, this will get the next field and set the
	* internal iteration to the next field.
	*/
	virtual gmsec::Status CALL_TYPE GetNextField(gmsec::Field &field);

#endif /* SUBCLASS_BASEMESSAGE */

};

#endif	// KafkaMessage_h
