SRCS=AtariEm.cpp MemorySpace.cpp MemoryDevice.cpp Device.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

CC=gcc
CXX=g++
RM=rm -f

CPPFLAGS=-g -pthread
LDFLAGS=-g 
LDLIBS=

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