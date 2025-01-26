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

/* NOTE: This function will NOT check if the block has been initialised as a
   record or an index block. It will copy whatever content is there in that
   disk block to the buffer.
   Also ensure that all the methods accessing and updating the block's data
   should call the loadBlockAndGetBufferPtr() function before the access or
   update is done. This is because the block might not be present in the
   buffer due to LRU buffer replacement. So, it will need to be bought back
   to the buffer before any operations can be done.
 */
int BlockBuffer::loadBlockAndGetBufferPtr(unsigned char ** buffPtr) 
{
    /* check whether the block is already present in the buffer
       using StaticBuffer.getBufferNum() */
    int bufferNum = StaticBuffer::getBufferNum(this->blockNum);

    // if present (!=E_BLOCKNOTINBUFFER),
        // set the timestamp of the corresponding buffer to 0 and increment the
        // timestamps of all other occupied buffers in BufferMetaInfo.
      if(bufferNum!=E_BLOCKNOTINBUFFER)
      {
        
        for(int i=0;i<BUFFER_CAPACITY;i++)
        {
          if(StaticBuffer::metainfo[i].free==false)
          {
            StaticBuffer::metainfo[i].timeStamp++;
          }
        }
        StaticBuffer::metainfo[bufferNum].timeStamp=0;
      }
      else
      {
        bufferNum = StaticBuffer::getFreeBuffer(this->blockNum);

        if (bufferNum == E_OUTOFBOUND)
        {
          return E_OUTOFBOUND;
        }
            

        Disk::readBlock(StaticBuffer::blocks[bufferNum], this->blockNum);
      }

    // else
        // get a free buffer using StaticBuffer.getFreeBuffer()

        // if the call returns E_OUTOFBOUND, return E_OUTOFBOUND here as
        // the blockNum is invalid

        // Read the block into the free buffer using readBlock()

    // store the pointer to this buffer (blocks[bufferNum]) in *buffPtr
    *buffPtr = StaticBuffer::blocks[bufferNum];
    return SUCCESS;

    // return SUCCESS;
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
int RecBuffer::setRecord(union Attribute *rec, int slotNum) 
{
    unsigned char *bufferPtr;
    /* get the starting address of the buffer containing the block
       using loadBlockAndGetBufferPtr(&bufferPtr). */
       int k=loadBlockAndGetBufferPtr(&bufferPtr);

    // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
        // return the value returned by the call.
        if(k!=SUCCESS)
        {
          return k;
        }

    /* get the header of the block using the getHeader() function */
    HeadInfo head1;
    this->getHeader(&head1);

    // get number of attributes in the block.
    int attrs=head1.numAttrs;

    // get the number of slots in the block.
    int slots=head1.numSlots;

    // if input slotNum is not in the permitted range return E_OUTOFBOUND.
    if(slotNum<0 || slotNum>=slots)
    {
      return E_OUTOFBOUND;
    }

    /* offset bufferPtr to point to the beginning of the record at required
       slot. the block contains the header, the slotmap, followed by all
       the records. so, for example,
       record at slot x will be at bufferPtr + HEADER_SIZE + (x*recordSize)
       copy the record from `rec` to buffer using memcpy
       (hint: a record will be of size ATTR_SIZE * numAttrs)
    */
   int rec_size=ATTR_SIZE*attrs;
   int offset=HEADER_SIZE+slots+(rec_size*slotNum);
   memcpy(bufferPtr+offset,rec,rec_size);

    // update dirty bit using setDirtyBit()
    StaticBuffer::setDirtyBit(this->blockNum);

    /* (the above function call should not fail since the block is already
       in buffer and the blockNum is valid. If the call does fail, there
       exists some other issue in the code) */
       return SUCCESS;

    // return SUCCESS
}
