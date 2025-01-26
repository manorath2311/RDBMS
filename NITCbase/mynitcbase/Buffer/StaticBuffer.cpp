#include "StaticBuffer.h"
// the declarations for this class can be found at "StaticBuffer.h"

unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];

StaticBuffer::StaticBuffer() 
{

  for (int i=0; i<BUFFER_CAPACITY; i++) 
  {
    // set metainfo[bufferindex] with the following values
    //   free = true
    //   dirty = false
    //   timestamp = -1
    //   blockNum = -1
    metainfo[i].free=true;
    metainfo[i].dirty=false;
    metainfo[i].timeStamp=-1;
    metainfo[i].blockNum=-1;

  }
}

// write back all modified blocks on system exit
StaticBuffer::~StaticBuffer() 
{
  /*iterate through all the buffer blocks,
    write back blocks with metainfo as free=false,dirty=true
    using Disk::writeBlock()
    */
  for(int i=0;i<BUFFER_CAPACITY;i++)
  {
    if(metainfo[i].free==false && metainfo[i].dirty==true)
    {
        Disk::writeBlock(blocks[i],metainfo[i].blockNum);
    }
  }
}

/*
At this stage, we are not writing back from the buffer to the disk since we are
not modifying the buffer. So, we will define an empty destructor for now. In
subsequent stages, we will implement the write-back functionality here.
*/



int StaticBuffer::getFreeBuffer(int blockNum)
{
    // Check if blockNum is valid (non zero and less than DISK_BLOCKS)
    // and return E_OUTOFBOUND if not valid.
  if (blockNum < 0 || blockNum >=DISK_BLOCKS)
  {
    return E_OUTOFBOUND;
  }
  int allocatedBuffer=-1;

    // increase the timeStamp in metaInfo of all occupied buffers.
  
    for(int i=0;i<BUFFER_CAPACITY;i++)
    {
      if(metainfo[i].free==false)
      {
        metainfo[i].timeStamp++;
      }
    }
 
     bool bufferFound=false;
  for(int i=0;i<BUFFER_CAPACITY;i++)
  {
    if(metainfo[i].free==true)
    {
        allocatedBuffer=i;
        bufferFound = true;
        break;
    }
  }
  
  

    // let bufferNum be used to store the buffer number of the free/freed buffer.
    int bufferNum=allocatedBuffer;


    // iterate through metainfo and check if there is any buffer free

    // if a free buffer is available, set bufferNum = index of that free buffer.

    // if a free buffer is not available,
    //     find the buffer with the largest timestamp
    //     IF IT IS DIRTY, write back to the disk using Disk::writeBlock()
    //     set bufferNum = index of this buffer
    if (!bufferFound)
    {
        int maxTimeStamp = -1;
        for (int i= 0;i< BUFFER_CAPACITY;i++)
        {
            if (metainfo[i].timeStamp > maxTimeStamp)
            {
                maxTimeStamp = metainfo[i].timeStamp;
                bufferNum = i;
            }
        }
        if (metainfo[bufferNum].dirty)
        {
            Disk::writeBlock(blocks[bufferNum],metainfo[bufferNum].blockNum);
        }
    }

    // update the metaInfo entry corresponding to bufferNum with
    // free:false, dirty:false, blockNum:the input block number, timeStamp:0.
    metainfo[bufferNum].free = false;
    metainfo[bufferNum].dirty = false;
    metainfo[bufferNum].blockNum = blockNum;
    metainfo[bufferNum].timeStamp = 0;

    // return the bufferNum.
    return bufferNum;
}

/* Get the buffer index where a particular block is stored
   or E_BLOCKNOTINBUFFER otherwise
*/

int StaticBuffer::setDirtyBit(int blockNum)
{
    // find the buffer index corresponding to the block using getBufferNum().
    int buffind=getBufferNum(blockNum);

    // if block is not present in the buffer (bufferNum = E_BLOCKNOTINBUFFER)
    //     return E_BLOCKNOTINBUFFER
    if(buffind==E_BLOCKNOTINBUFFER)
    {
      return E_BLOCKNOTINBUFFER;
    }

    // if blockNum is out of bound (bufferNum = E_OUTOFBOUND)
    //     return E_OUTOFBOUND
    if(buffind==E_OUTOFBOUND)
    {
      return E_OUTOFBOUND;
    }

    // else
    //     (the bufferNum is valid)
    //     set the dirty bit of that buffer to true in metainfo
    metainfo[buffind].dirty=true;

    return SUCCESS;

    // return SUCCESS
}
int StaticBuffer::getBufferNum(int blockNum)
{
  // Check if blockNum is valid (between zero and DISK_BLOCKS)
  // and return E_OUTOFBOUND if not valid.
    if(blockNum<0 || blockNum>=DISK_BLOCKS)
    {
        return E_OUTOFBOUND;
    }

  // find and return the bufferIndex which corresponds to blockNum (check metainfo)
  for(int i=0;i<BUFFER_CAPACITY;i++)
  {
     if(metainfo[i].blockNum==blockNum)
     {
         return i;
    }
  }

  // if block is not in the buffer
  return E_BLOCKNOTINBUFFER;
}
