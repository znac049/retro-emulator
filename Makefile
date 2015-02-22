SRCS=AtariEm.cpp MemoryDevice.cpp CPU.cpp CPUState.cpp \
	Instructions.cpp Device.cpp MemoryMap.cpp Pokey.cpp \
	ADlxDigitalOutputs.cpp

OBJS=$(subst .cpp,.o,$(SRCS))

CC=gcc
CXX=g++
RM=rm -f

CPPFLAGS=-g -pthread
LDFLAGS=-g 
LDLIBS=-lrt

all: AtariEm

AtariEm: $(OBJS)
	$(CXX) $(LDFLAGS) -o AtariEm $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS) *~ AtariEm 

dist-clean: clean
	$(RM) *~ .depend

include .depend