MODULES= CPUs Debugger Devices Machines Misc UI

SRCS=gem.cpp \
	Debug.cpp \
	MemoryMap.cpp \
	Devices/Device.cpp \
	Devices/MemoryDevice.cpp Devices/RegisterBasedDevice.cpp \
	Machines/Machine.cpp \
	Machines/ADMachine.cpp \
	CPUs/CPU.cpp \
	CPUs/CPU6502.cpp \
	UI/CLI.cpp \
	Misc/AddressHash.cpp \
	Misc/PointTable.cpp \
	Misc/Timer.cpp \
	Misc/TimeStamp.cpp \
	Misc/Radix.cpp

OBJS=$(subst .cpp,.o,$(addprefix build/,$(notdir $(SRCS))))

CC=gcc
CXX=g++
RM=rm -f

CXXFLAGS=-g -O0 -pthread $(addprefix -I,$(MODULES))
LDFLAGS=-g 
LDLIBS=-lrt -lncurses -lstdc++

vpath %.cpp $(MODULES)

build/%.o: %.cpp
	$(CC) -c $(CXXFLAGS) $< -o build/$(notdir $@)

all: builddir gem

builddir:
	mkdir -p build

gem: $(OBJS)
	$(CC) $(LDFLAGS) -o gem $(OBJS) $(LDLIBS) 

clean:
	$(RM) $(OBJS) build/* *~ gem

dist-clean: clean
	$(RM) *~ .depend

