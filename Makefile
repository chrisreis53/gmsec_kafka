# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# All Rights Reserved.

#	Makefile - kafka middleware wrapper

GMSEC_HOME	= ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME	= gmsec_kafka

TARGET		= lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR		= ./src

LOCAL_DEFS		= -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS		= -I./include

CXXFLAGS	+= $(LOCAL_DEFS) $(LOCAL_INCS)

LIBDIRS		=

LIBS		=

#
SRCDIR		= src

OBJECTS		= \
	$(OBJDIR)/gmsec_kafka.o \
	$(OBJDIR)/KafkaConnection.o \
	#$(OBJDIR)/KafkaMessage.o


# default: clean library
default: clean library

library: $(BINDIR)/$(TARGET)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(BASE_LDFLAGS) $(OBJECTS) $(LIBDIRS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@
