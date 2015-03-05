/**
 * @file IntelHexIO.cpp
 *
 * Read and write data in Intel Hex format
 *
 * @author Bob Green
*/

#include "IntelHex.h"

bool IntelHex::writeBytes(FILE *fd, char *buffer, int numBytes, int address)
{
  while (numBytes > 0) {
    int chunkSize = (numBytes > 16) ? 16 : numBytes;
    int sum = chunkSize + (address & 0xff) + ((address >> 8) & 0xff);

    fprintf(fd, ":%02X%04X00", chunkSize, address);

    for (int i=0; i<chunkSize; i++) {
      sum += buffer[i];
      fprintf(fd, "%02X", buffer[i]); 
    }

    sum = (~sum & 0xff) + 1;
    fprintf(fd, "%02X\n",sum);

    address += chunkSize;
    numBytes -= chunkSize;
    buffer += chunkSize;
  }

  fputs(":00000001FF\n", fd);

  return true;
}

int IntelHEX::readBytes(FILE *fd,char *buffer, int bufLen)
{
  char line[MAX_IHEX_LINE];
  int numRead = 0;
  int byteCount;
  int addr;
  int recordType;

  while ((bufLen > 0) & (!feof(fd))) {
    if (fgets(line, MAX_IHEX_LINE, fd)) {
      if (line[0] != ':') {
	break;
      }
      
      byteCount = hexByte(buffer);
      addr = (hexByte(buffer+2) << 8) | hexByte(buffer+4);
      recordType = hexByte(buffer+6);

      switch (recordType) {
      case 0:
	for (int i=0; i<byteCount; i++) {
	  if ((addr < 0) || (addr >= buffLen)) {
	    return numRead;
	  }

	  buffer[addr] = hexByte(buffer[buffer+8+(i<<1)]);
	  addr++;
	  numRead++;
	}
	break;

      case 1:
	return numRead;
	break;

      default:
	break;
      }
    }
    else {
      break;
    }
  }

    return numRead;
}

int IntelHEX::hexByte(const char *buff)
{
  
}
