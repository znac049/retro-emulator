SRCS=AtariEm.cpp MemoryDevice.cpp CPU.cpp CPUState.cpp \
	Instructions.cpp Device.cpp MemoryMap.cpp Pokey.cpp \
	ADlxOutputs.cpp \
	ADlxSwitches.cpp ADlxInputs.cpp ADlxOptionSwitches.cpp \
	VectorGenerator.cpp StartVGDevice.cpp 

OBJS=$(subst .cpp,.o,$(SRCS))

CC=gcc
CXX=g++
RM=rm -f

CPPFLAGS=-g -pthread
LDFLAGS=-g 
LDLIBS=-lrt -lncurses

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
