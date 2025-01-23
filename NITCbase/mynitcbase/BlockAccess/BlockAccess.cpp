#include "BlockAccess.h"
#include <cstdlib>
#include <cstring>
RecId BlockAccess::linearSearch(int relId, char attrName[ATTR_SIZE], union Attribute attrVal, int op)
{
    
  /*  
    RecId prevRecId;
    RelCacheTable::getSearchIndex(relId, &prevRecId);

    int block, slot;


    if (prevRecId.block == -1 && prevRecId.slot == -1)
    {
 
        RelCatEntry relCatEntry;
        RelCacheTable::getRelCatEntry(relId, &relCatEntry);

        block = relCatEntry.firstBlk;
        slot = 0;
    }
    else
    {
        block = prevRecId.block;
        slot = prevRecId.slot + 1;
    }

   
    while (block != -1)
    {

        RecBuffer recBuffer(block);

        HeadInfo head;
        recBuffer.getHeader(&head);
        unsigned char slotMap[head.numSlots];
        recBuffer.getSlotMap(slotMap);

        if (slot >= head.numSlots)
        {

            block = head.rblock;
            slot = 0;

            continue; 
        }

        int numAttrs = head.numAttrs;

        Attribute record[numAttrs];
        recBuffer.getRecord(record, slot);

        if (slotMap[slot] == SLOT_UNOCCUPIED)
        {
   
            slot++;
            continue;
        }

        AttrCatEntry attrCatEntry;
        AttrCacheTable::getAttrCatEntry(relId, attrName, &attrCatEntry);

        Attribute recordAttrVal = record[attrCatEntry.offset];
        int cmpVal; 
        cmpVal = compareAttrs(recordAttrVal, attrVal, attrCatEntry.attrType);
        if ((op == NE && cmpVal != 0) ||(op == LT && cmpVal < 0) ||  (op == LE && cmpVal <= 0) || (op == EQ && cmpVal == 0) || (op == GT && cmpVal > 0) || (op == GE && cmpVal >= 0)    )
        {
    
            prevRecId = RecId{block, slot};
            RelCacheTable::setSearchIndex(relId, &prevRecId);
            return RecId{block, slot};
        }

        slot++;
    }

    // no record in the relation with Id relid satisfies the given condition
    */
    return RecId{-1, -1};
}