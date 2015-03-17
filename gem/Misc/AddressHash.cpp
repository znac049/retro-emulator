/*
 * AddressHash.cpp
 *
 *  Created on: 17 Mar 2015
 *      Author: bob
 */

/*
 * designed for a 16-bit address space - do not use it for anything bigger - it will break!
 */
#include "AddressHash.h"

AddressHash::AddressHash()
{
	hashRoot = nullptr;

	hits = misses = 0;
}

AddressHash::~AddressHash()
{
	if (hashRoot != nullptr) {
		for (int i=0; i<HashEntrySize; i++) {
			if (hashRoot[i] != nullptr) {
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
		ent[i] = nullptr;
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

	if (hashRoot == nullptr) {
		hashRoot = newEntry();
	}

	if (hashRoot[key1] == nullptr) {
		hashRoot[key1] = newEntry();
	}

	hashRoot[key1][key2] = val;
}

int AddressHash::get(int addr)
{
	int val = 0;

	if (hashRoot != nullptr) {
		byte key1 = addr & 0xff;

		if (hashRoot[key1] != nullptr) {
			byte key2 = (addr >> 8) & 0xff;

			val = hashRoot[key1][key2];
		}
	}

	return val;
}

void AddressHash::clearAll()
{
	if (hashRoot != nullptr) {
		for (int i=0; i<HashEntrySize; i++) {
			if (hashRoot[i] != nullptr) {
				delete hashRoot[i];
			}
		}

		delete hashRoot;
	}

	hashRoot = nullptr;
}
