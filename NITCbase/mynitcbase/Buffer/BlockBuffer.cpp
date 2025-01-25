#include "BlockBuffer.h"

#include <cstdlib>
#include <cstring>

// the declarations for these functions can be found in "BlockBuffer.h"

BlockBuffer::BlockBuffer(int blockNum)
{
  this->blockNum=blockNum;
}

// calls the parent class constructor

RecBuffer::RecBuffer(int blockNum) : BlockBuffer::BlockBuffer(blockNum) {}


/*
Used to get the header of the block into the location pointed to by `head`
NOTE: this function expects the caller to allocate memory for `head`
*/
int BlockBuffer::getHeader(struct HeadInfo *head)
{

  unsigned char *bufferPtr;
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS)
  {
    return ret;
  }
  memcpy(&head->numSlots, bufferPtr + 24, 4);
  memcpy(&head->numEntries,16+bufferPtr, 4);
  memcpy(&head->numAttrs, 20+bufferPtr, 4);
  memcpy(&head->rblock,12+bufferPtr, 4);
  memcpy(&head->lblock,8+bufferPtr, 4);

  return SUCCESS;

}

/*
Used to get the record at slot `slotNum` into the array `rec`
NOTE: this function expects the caller to allocate memory for `rec`
*/
int RecBuffer::getRecord(union Attribute *rec, int slotNum)
{
  unsigned char *bufferPtr;
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS)
  {
    return ret;
  }
    struct HeadInfo head;

  this->getHeader(&head);

  int attrCount = head.numAttrs;
  int slotCount = head.numSlots;

  unsigned char *slotPointer ;
  int recordSize = attrCount * ATTR_SIZE;
    slotPointer = bufferPtr+32+slotCount+(recordSize*(slotNum));

   memcpy(rec,slotPointer,recordSize);

  return SUCCESS;


}

/*
Used to load a block to the buffer and get a pointer to it.
NOTE: this function expects the caller to allocate memory for the argument
*/
int BlockBuffer::loadBlockAndGetBufferPtr(unsigned char **buffPtr)
{
  // check whether the block is already present in the buffer using StaticBuffer.getBufferNum()
  int bufferNum = StaticBuffer::getBufferNum(this->blockNum);

  if (bufferNum == E_BLOCKNOTINBUFFER)
  {
    bufferNum = StaticBuffer::getFreeBuffer(this->blockNum);

    if (bufferNum == E_OUTOFBOUND)
    {
      return E_OUTOFBOUND;
    }

    Disk::readBlock(StaticBuffer::blocks[bufferNum], this->blockNum);
  }

  // store the pointer to this buffer (blocks[bufferNum]) in *buffPtr
  *buffPtr = StaticBuffer::blocks[bufferNum];

  return SUCCESS;
}
/* used to get the slotmap from a record block
NOTE: this function expects the caller to allocate memory for `*slotMap`
*/
/* used to get the slotmap from a record block
NOTE: this function expects the caller to allocate memory for `*slotMap`
*/
int RecBuffer::getSlotMap(unsigned char *slotMap) 
{
  unsigned char *bufferPtr;

  // get the starting address of the buffer containing the block using loadBlockAndGetBufferPtr().
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) 
  {
    return ret;
  }

  struct HeadInfo head;
  // get the header of the block using getHeader() function
  getHeader(&head);


  int slotCount = head.numSlots;/* number of slots in block from header */

  // get a pointer to the beginning of the slotmap in memory by offsetting HEADER_SIZE
  unsigned char *slotMapInBuffer = bufferPtr + HEADER_SIZE;

  // copy the values from `slotMapInBuffer` to `slotMap` (size is `slotCount`)
  memcpy(slotMap,slotMapInBuffer,slotCount);

  return SUCCESS;
}

int compareAttrs(union Attribute attr1, union Attribute attr2, int attrType)
{

    double diff;
    if(attrType==STRING)
    {
      diff=strcmp(attr1.sVal,attr2.sVal);
    }
    else
    {
      diff=attr1.nVal-attr2.nVal;
    }
    if(diff==0)
    {
      return 0;
    }
    else if(diff>0)
    {
      return 1;
    }
    return -1;

}
