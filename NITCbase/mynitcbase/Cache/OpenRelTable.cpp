#include "OpenRelTable.h"
#include<stdlib.h>
#include <cstring>
#include <cstring>
#include <cstdio>


OpenRelTable::OpenRelTable()
{

  // initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN;i++)
  {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
  }

  /************ Setting up Relation Cache entries ************/
  // (we need to populate relation cache with entries for the relation catalog
  //  and attribute catalog.)

  /**** setting up Relation Catalog relation in the Relation Cache Table****/
  RecBuffer relCatBlock(RELCAT_BLOCK);

  Attribute relCatRecord[RELCAT_NO_ATTRS];
  relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

  struct RelCacheEntry relCacheEntry;
  RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
  relCacheEntry.recId.block = RELCAT_BLOCK;
  relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

  // allocate this on the heap because we want it to persist outside this function
  RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;

  /**** setting up Attribute Catalog relation in the Relation Cache Table ****/

  // set up the relation cache entry for the attribute catalog similarly
  // from the record at RELCAT_SLOTNUM_FOR_ATTRCAT

   relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_ATTRCAT);
   //struct RelCacheEntry relCacheEntry;
  RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
  relCacheEntry.recId.block = RELCAT_BLOCK;
  relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_ATTRCAT;

  // allocate this on the heap because we want it to persist outside this function
  RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[ATTRCAT_RELID]) = relCacheEntry;

  // set the value at RelCacheTable::relCache[ATTRCAT_RELID]


  /************ Setting up Attribute cache entries ************/
  // (we need to populate attribute cache with entries for the relation catalog
  //  and attribute catalog.)

  /**** setting up Relation Catalog relation in the Attribute Cache Table ****/

  RecBuffer obj1(RELCAT_BLOCK);
   Attribute at[4];
  obj1.getRecord(at,2);
  struct RelCacheEntry RR1;
  struct RelCatEntry RR2;

  RelCacheTable::recordToRelCatEntry(at,&(RR1.relCatEntry));
RR1.recId.block=RELCAT_BLOCK;
RR1.recId.slot=2;
RelCacheTable::relCache[2] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[2]) = RR1;






AttrCacheEntry* relhead=nullptr;
AttrCacheEntry* relprev=nullptr;
RecBuffer relcat(ATTRCAT_BLOCK);
Attribute relattr[6];

for(int i=0;i<6;i++)
{
  AttrCacheEntry* x=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
  relcat.getRecord(relattr,i);
  AttrCacheTable::recordToAttrCatEntry(relattr,&(x->attrCatEntry));
  x->recId.slot=i;
  x->recId.block=RELCAT_BLOCK;
  x->next=nullptr;
  if(relprev==nullptr)
  {
    relhead=x;
  }
  else
  {
    relprev->next=x;
  }
  relprev=x;
}




AttrCacheTable::attrCache[RELCAT_RELID]=relhead;























/* head of the linked list */;

  /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/

  // set up the attributes of the attribute cache similarly.
  // read slots 6-11 from attrCatBlock and initialise recId appropriately

  // set the value at AttrCacheTable::attrCache[ATTRCAT_RELID]
    /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/

  // Initialize the attribute cache for ATTRCAT_RELID
  AttrCacheEntry* attrHead = nullptr;
  AttrCacheEntry* attrPrev = nullptr;


  RecBuffer attrCatBlock2(ATTRCAT_BLOCK);

  Attribute attrRecord1[ATTRCAT_NO_ATTRS];

  for (int i = RELCAT_NO_ATTRS; i <RELCAT_NO_ATTRS+ATTRCAT_NO_ATTRS;i++)
  {
    attrCatBlock2.getRecord(attrRecord1,i);

    AttrCacheEntry* current = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    //memset(current, 0, sizeof(AttrCacheEntry));
    AttrCacheTable::recordToAttrCatEntry(attrRecord1,&current->attrCatEntry);
    current->recId.block = ATTRCAT_BLOCK;
    current->recId.slot = i;
    current->next = nullptr;

    if (attrPrev)
    {
      attrPrev->next = current;
    }
    else
    {
      attrHead = current;
    }
    attrPrev = current;



  }



  AttrCacheTable::attrCache[ATTRCAT_RELID] = attrHead;



AttrCacheEntry* relhead2=nullptr;
AttrCacheEntry* relprev2=nullptr;
RecBuffer relcat2(ATTRCAT_BLOCK);
Attribute relattr2[6];

for(int i=12;i<16;i++)
{
  AttrCacheEntry* x=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
  relcat2.getRecord(relattr2,i);
  AttrCacheTable::recordToAttrCatEntry(relattr2,&(x->attrCatEntry));
  x->recId.slot=i;
  x->recId.block=ATTRCAT_BLOCK;
  x->next=nullptr;
  if(relprev2==nullptr)
  {
    relhead2=x;
  }
  else
  {
    relprev2->next=x;
  }
  relprev2=x;
}




AttrCacheTable::attrCache[2]=relhead2;




/*

  //RecBuffer attrCatBlock(ATTRCAT_BLOCK);
  //Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
  RecBuffer RelCatBuffer(RELCAT_BLOCK);
  HeadInfo RelCatHeader;
  RelCatBuffer.getHeader(&RelCatHeader);
  int slot=-1;
  int entries=-1;
  int block=RELCAT_BLOCK;
  for(int i=0;i<RelCatHeader.numEntries;i++)
  {
    Attribute RelCatRecord[RELCAT_NO_ATTRS];
    RelCatBuffer.getRecord(RelCatRecord,i);
    if(strcmp(RelCatRecord[RELCAT_REL_NAME_INDEX].sVal,"Students")==0)
    {
      slot=i;
      entries=RelCatRecord[RELCAT_NO_ATTRS].nVal;
      break;
    }
  }
  struct RelCacheEntry cac1;
  Attribute record[RELCAT_NO_ATTRS];
  relCatBlock.getRecord(record,slot);
  RelCacheTable::recordToRelCatEntry(record,&cac1.relCatEntry);
  cac1.recId.block=RELCAT_BLOCK;
  cac1.recId.slot=slot;
  RelCacheTable::relCache[2]=(struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[2])=cac1;


*/



}



OpenRelTable::~OpenRelTable()
{
  for(int i=0;i<MAX_OPEN;i++)
  {
    RelCacheEntry* current = RelCacheTable::relCache[i];
    if(current)
    {
      free(current);
    }
  }

  for (int i = 0; i < MAX_OPEN;i++)
  {
    AttrCacheEntry* current = AttrCacheTable::attrCache[i];
    if(current)
    {
    while (current)
    {
      AttrCacheEntry* next = current->next;
      free(current);
      current = next;
    }
    AttrCacheTable::attrCache[i] = nullptr;
    }
  }

}
