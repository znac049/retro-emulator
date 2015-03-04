SRCS=gem.cpp \
	Device.cpp DeviceListener.cpp RegisterBasedDevice.cpp \
	ADInput.cpp ADInputs.cpp ADSin0.cpp ADSin1.cpp \
	ADlxOptionSwitches.cpp \
	ADOutput.cpp ADOutputs.cpp \
	Pokey.cpp MemoryDevice.cpp \
	WatchDog.cpp \
	Processor.cpp \
	R6502.cpp R6502State.cpp R6502Instructions.cpp \
	i8080.cpp \
	Window.cpp CodeWindow.cpp \
	Console.cpp \
	MemoryMap.cpp \
	Timer.cpp TimerListener.cpp \
	VectorGenerator.cpp \
	AsteroidsDeluxe.cpp

SRCS=gem.cpp \
	ADMachine.cpp Machine.cpp \
	Processor.cpp R6502.cpp R6502State.cpp R6502Instructions.cpp \
	MemoryMap.cpp \
	Device.cpp DeviceListener.cpp RegisterBasedDevice.cpp \
	Pokey.cpp MemoryDevice.cpp

OBJS=$(subst .cpp,.o,$(SRCS))

CC=gcc
CXX=g++
RM=rm -f

CPPFLAGS=-g -pthread -Icpus -Idevices -Isystems -Iui
LDFLAGS=-g 
LDLIBS=-lrt -lncurses

all: gem

gem: $(OBJS)
	$(CXX) $(LDFLAGS) -o gem $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS) *~ gem

dist-clean: clean
	$(RM) *~ .depend

include .depend
