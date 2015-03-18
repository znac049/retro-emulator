#include "AddressHash.h"

#include "../gem.h"

AddressHash::AddressHash()
{
  hashRoot = (int **)0;

  hits = misses = 0;
}

AddressHash::~AddressHash()
{
  if (hashRoot != NULL) {
    for (int i=0; i<HashEntrySize; i++) {
      if (hashRoot[i] != NULL) {
	delete hashRoot[i];
      }
    }

    delete hashRoot;
  }
}
int *AddressHash::newEntry()
{
  int *ent = new int[HashEntrySize];

  for (int i=0; i<HashEntrySize; i++) {
    ent[i] = NULL;
  }

  return ent;
}

void AddressHash::set(int addr, int val)
{
  byte key1 = addr & 0xff;
  byte key2 = (addr >> 8) & 0xff;

  if (addr & ~0xffff) {
    throw "this hash can only handle 16-bit addresses";
  }

  if (hashRoot == NULL) {
    //hashRoot = newEntry();
    hashRoot = new int *[HashEntrySize];
    for (int i=0; i<HashEntrySize; i++) {
      hashRoot[i] = NULL;
    }
  }

  if (hashRoot[key1] == NULL) {
    hashRoot[key1] = newEntry();
  }

  hashRoot[key1][key2] = val;
}

int AddressHash::get(int addr)
{
  int val = 0;

  if (hashRoot != NULL) {
    byte key1 = addr & 0xff;

    if (hashRoot[key1] != NULL) {
      byte key2 = (addr >> 8) & 0xff;

      val = hashRoot[key1][key2];
    }
  }

  return val;
}

void AddressHash::clearAll()
{
  if (hashRoot != NULL) {
    for (int i=0; i<HashEntrySize; i++) {
      if (hashRoot[i] != NULL) {
	delete hashRoot[i];
      }
    }

    delete hashRoot;
  }

  hashRoot = NULL;
}
