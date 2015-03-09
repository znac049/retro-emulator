SRCS=gasm.cpp \
	ALine.cpp \
	ListNode.cpp \
	Label.cpp LabelTable.cpp \
	Assembler.cpp 

OBJS=$(subst .cpp,.o,$(SRCS))

CC=gcc
CXX=g++
RM=rm -f

CPPFLAGS=-g -pthread
LDFLAGS=-g 
LDLIBS=

all: gasm

gasm: $(OBJS)
	$(CXX) $(LDFLAGS) -o gasm $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS) *~ gasm

dist-clean: clean
	$(RM) *~ .depend

include .depend
