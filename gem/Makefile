MODULES= CPUs Devices Machines Debugger UI

SRCS=gem.cpp \
	MemoryMap.cpp \
	Devices/Device.cpp \
	Devices/MemoryDevice.cpp Devices/RegisterBasedDevice.cpp \
	Devices/Pokey.cpp \
	Machines/Machine.cpp \
	Machines/ADMachine.cpp \
	CPUs/CPU.cpp CPUs/CPUState.cpp \
	CPUs/CPU6502.cpp CPUs/State6502.cpp

OBJS=$(subst .cpp,.o,$(addprefix build/,$(notdir $(SRCS))))

CC=gcc
CXX=g++
RM=rm -f

CXXFLAGS=-g -pthread $(addprefix -I,$(MODULES))
LDFLAGS=-g 
LDLIBS=-lrt -lncurses

vpath %.cpp $(MODULES)

build/%.o: %.cpp
	$(CC) -c $(CXXFLAGS) $< -o build/$(notdir $@)

all: builddir gem

builddir:
	mkdir -p build

gem: $(OBJS)
	$(CXX) $(LDFLAGS) -o gem $(OBJS) $(LDLIBS) 

clean:
	$(RM) $(OBJS) build/ *~ gem

dist-clean: clean
	$(RM) *~ .depend

