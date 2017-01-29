# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# All Rights Reserved.

#	Makefile - kafka middleware wrapper

GMSEC_HOME	= ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME	= gmsec_kafka

TARGET		= lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR		= ./src

SUPDIR       = $(SUPPORT)/kafka

LOCAL_DEFS		= -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS   = -I$(GMSEC_HOME)/include \
                -I$(SUPDIR)/include \
                -I./include \

CXXFLAGS	+= $(LOCAL_DEFS) $(LOCAL_INCS)

LDFLAGS   += -L$(SUPDIR)/include -L/usr/local/lib -lrdkafka++ -lrdkafka -lpthread -lrt -lz
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
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $(LDFLAGS) $< -o $@
