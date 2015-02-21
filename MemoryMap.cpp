#include <stdio.h>

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
  printf("Connecting device '%s' at base address $%04x\n", dev->getName(), addr);

  add(dev, addr, addr + dev->getSize() - 1);

  return true;
}

byte MemoryMap::peek(int addr)
{
  Node *ent = findDevice(addr);

  if (ent != NULL) {
    byte b = ent->device->peek(addr - ent->startAddress);

    printf("Rd $%04x (%s) = $%02x\n", addr, ent->device->getName(), b);
  }

  return 0;
}

void MemoryMap::poke(int addr, byte b)
{
  Node *ent = findDevice(addr);

  if (ent != NULL) {
    ent->device->poke(addr - ent->startAddress, b);

    //printf("Wr $%04x (%s) = $%02x\n", addr, ent->device->getName(), b);
  }
}

word MemoryMap::peekw(int addr)
{
  return ( (((word) peek(addr+1))<<8) | peek(addr) );
}

void MemoryMap::pokew(int addr, word w)
{
  poke(addr,   w & 0xff);
  poke(addr+1, (w>>8) & 0xff);
}

void MemoryMap::add(Device *dev, int from, int to)
{
  Node *ent;

  if (from > to) {
    throw "start is after end when adding device to memory map";
  }

  ent = new Node();

  ent->startAddress = from;
  ent->endAddress = to;
  ent->device = dev;

  ent->before = ent->after = NULL;

  head = insert(ent, head);
}

MemoryMap::Node *MemoryMap::insert(Node *ent, Node *node)
{
  if (node == NULL) {
    return ent;
  }

  if (node->startAddress == ent->startAddress) {
    throw "Overlapping devices when adding device to memory map";
  }

  if (ent->startAddress < node->startAddress) {
    node->before = insert(ent, node->before);
  }
  else {
    node->after = insert(ent, node->after);
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

    if (addr < ent->startAddress) {
      ent = ent->before;
    }
    else {
      ent = ent->after;
    }
  }

  printf("No device in memory map at address $%04x\n", addr);
  return NULL;
}

void MemoryMap::dump()
{
  printf("Memory Map:\n");

  dumpNode(head);
}

void MemoryMap::dumpNode(Node *ent)
{
  if (ent->before != NULL) {
    dumpNode(ent->before);
  }

  printf("%-20s ", ent->device->getName());

  if (ent->startAddress == ent->endAddress) {
    printf("$%04x         ", ent->startAddress);
  }
  else {
    printf("$%04x - $%04x ", ent->startAddress, ent->endAddress);
  }

  printf("(%d bytes)\n", ent->device->getSize());

  if (ent->after != NULL) {
    dumpNode(ent->after);
  }
}
