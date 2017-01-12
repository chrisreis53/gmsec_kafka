/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * All Rights Reserved.
 */

/* @file gmsec_kafka.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#ifndef GMSEC_KAFKA_H
#define GMSEC_KAFKA_H

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

//#include <zmq.h>


// Use an extra level of indirection to expand integer macros before concatenating them
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifdef WIN32

	#include <windows.h>

	#ifdef GMSEC_KAFKA_EXPORTS
		#define GMSEC_KAFKA_API __declspec(dllexport)
	#else
		#define GMSEC_KAFKA_API __declspec(dllimport)
	#endif

	/* disable invalid STL waring for windows targets */
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4786 )

#else // All other platforms make this macro invisible

	#define GMSEC_KAFKA_API

#endif


//Constants
//#define KAFKA_VERSION_STRING "gmsec_kafka" STR(KAFKA_VERSION_MAJOR) STR(KAFKA_VERSION_MINOR) STR(KAFKA_VERSION_PATCH)
#define KAFKA_REPLY_ADDRESS "KAFKA-REPLY-ADDRESS"


// GMSEC_LIBROOTNAME is defined in the building Makefile or mak/dsp file.
#ifdef GMSEC_LIBROOTNAME
	// This makes a "" string from the given symbol.
	#define makeString(s) (#s)
	// This evaluates the symbol before making a string out of it.
	#define makeStringFromValue(s) makeString(s)
	// That way, I can make a string which has the value of the macro,
	// as opposed to a string which has the value of the macro name.
	// No more "" in Makefiles and/or mak/dsw files.
	#define KAFKA_VERSION_STRING makeStringFromValue(GMSEC_LIBROOTNAME)
#else
	// Or, in the case of the original 6.5 release, this.
	#define KAFKA_VERSION_STRING "gmsec_kafka"
#endif


#ifdef __cplusplus
extern "C"
{
#endif

GMSEC_KAFKA_API void createConnection(const gmsec::api::Config* config, gmsec::api::internal::ConnectionInterface** connIf , gmsec::api::Status* status);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
