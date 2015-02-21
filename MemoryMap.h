#ifndef _MEMORYMAP_H_
#define _MEMORYMAP_H_

#include "AtariEm.h"
#include "Device.h"

class MemoryMap
{
 private:
  int memorySize;

  struct Node {
    int startAddress;
    int endAddress;
    Device *device;

    Node *before;
    Node *after;
  };

  typedef struct Node Node;
  Node *head;

 private:
  Node *insert(Node *ent, Node *node);
  Node *findDevice(int addr);

 public:
  MemoryMap(int size);
  ~MemoryMap();

  bool connect(Device *dev, int addr);

  byte peek(int addr);
  void poke(int addr, byte b);

  word peekw(int addr);
  void pokew(int addr, word w);

  void add(Device *dev, int from, int to);
};

#endif
