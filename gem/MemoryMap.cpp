#include <stdio.h>

#include "Debug.h"
#include "MemoryMap.h"

MemoryMap::MemoryMap(int size)
{
  head = NULL;

  memorySize = size;
}

MemoryMap::~MemoryMap()
{
  // TODO
}

bool MemoryMap::connect(Device *dev, int addr)
{
  add(dev, addr, addr + dev->getSize() - 1);

  return true;
}

byte MemoryMap::peek(int addr)
{
  byte b = 0;
  Node *ent = findDevice(addr);

  if (ent != NULL) {
    b = ent->device->peek(addr - ent->startAddress);
  }

  return b;
}

void MemoryMap::poke(int addr, byte b)
{
  Node *ent = findDevice(addr);

  if (ent != NULL) {
    ent->device->poke(addr - ent->startAddress, b);
  }
}

int MemoryMap::peekw(int addr)
{
  int w = 0;
  Node *ent = findDevice(addr);

  if (ent != NULL) {
    int ad = addr - ent->startAddress;

    w = ent->device->peek(ad+1)<<8;
    w |= ent->device->peek(ad);
  }

  return w;
}

void MemoryMap::pokew(int addr, int w)
{
  Node *ent = findDevice(addr);

  if (ent != NULL) {
    int ad = addr - ent->startAddress;

    ent->device->poke(ad,   w & 0xff);
    ent->device->poke(ad+1, (w>>8) & 0xff);
  }
}

void MemoryMap::add(Device *dev, int from, int to)
{
  Node *ent;
  Node *prev;
  Node *node;

  if (from > to) {
    Debug::logf(10, "start is after end when adding device to memory map\n");

    throw "start is after end when adding device to memory map";
  }

  ent = new Node();

  ent->startAddress = from;
  ent->endAddress = to;
  ent->device = dev;

  ent->next = NULL;

  if (head == NULL) {
    head = ent;
  }
  else {
    for (node = head; node != NULL; node = node->next) {
      if (to < node->startAddress) {
	if (node == head) {
	  head = ent;
	}
	else {
	  prev->next = ent;
	}
	    
	ent->next = node;

	return;
      }

      prev = node;
    }

    prev->next = ent;
  }

  Debug::logf(10, "have we added or not???\n");
}

MemoryMap::Node *MemoryMap::insert(Node *ent, Node *node)
{
  if (node == NULL) {
    return ent;
  }

  if (node->startAddress == ent->startAddress) {
    throw "Overlapping devices when adding device to memory map";
  }

  if (ent->startAddress > node->startAddress) {
    return insert(ent, node->next);
  }
  else {
    ent->next = node;
    return ent;
  }

  return node;
}

MemoryMap::Node *MemoryMap::findDevice(int addr)
{
  Node *ent = head;

  while (ent != NULL) {
    if ((addr >= ent->startAddress) && (addr <= ent->endAddress)) {
      return ent;
    }

    ent = ent->next;

    if (ent->startAddress > addr) {
      return NULL;
    }
  }

  //printf("No device in memory map at address $%04x\n", addr);
  return NULL;
}

void MemoryMap::dump()
{
  printf("Memory Map:\n");

  dumpNode(head);
}

void MemoryMap::dumpNode(Node *ent)
{
  while (ent != NULL) {
    printf("%-20s ", ent->device->getName());

    if (ent->startAddress == ent->endAddress) {
      printf("$%04x         ", ent->startAddress);
    }
    else {
      printf("$%04x - $%04x ", ent->startAddress, ent->endAddress);
    }

    printf("(%d bytes)\n", ent->device->getSize());

    ent = ent->next;
  }
}

bool MemoryMap::getAddressName(char *str, int len, int addr)
{
  const char *fmt;
  Node *ent = findDevice(addr);

  if (addr <= 0xff) {
    fmt = "$%02X";
  }
  else {
    fmt = "$%04X";
  }

  if (ent != NULL) {
    if (!ent->device->getAddressName(str, len, addr - ent->startAddress)) {
      snprintf(str, len, fmt, addr);
    }
  }
  else {
    snprintf(str, len, fmt, addr);
  }
}
