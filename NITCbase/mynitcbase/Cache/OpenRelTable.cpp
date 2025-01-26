#include "OpenRelTable.h"

#include <cstring>
#include <cstdlib>

OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];
OpenRelTable::OpenRelTable()
{
  
    for(int i=0;i<MAX_OPEN;i++)
    {
        RelCacheTable::relCache[i]=nullptr;
        AttrCacheTable::attrCache[i]=nullptr;
    }
    //RelCache
    for(int i=0;i<MAX_OPEN;i++)
    {
      OpenRelTable::tableMetaInfo[i].free=true;
    }
    for(int i=0;i<2;i++)
    {
    RecBuffer relCatBuffer(RELCAT_BLOCK);
    Attribute relCatRecord[RELCAT_NO_ATTRS];
        relCatBuffer.getRecord(relCatRecord,i);
        RelCacheEntry relCacheEntry;
        RelCacheTable::recordToRelCatEntry(relCatRecord,&relCacheEntry.relCatEntry);
        relCacheEntry.dirty=false;
        relCacheEntry.recId.block=RELCAT_BLOCK;
        relCacheEntry.recId.slot=i;
        RelCacheTable::relCache[i]=(RelCacheEntry*)malloc(sizeof(RelCacheEntry));
        *(RelCacheTable::relCache[i])=relCacheEntry;
    }
    //AttrCache
   for(int i=0;i<2*RELCAT_NO_ATTRS;i++)
   {
    if(i<RELCAT_NO_ATTRS)
    {
       RecBuffer attrCatBuffer(ATTRCAT_BLOCK);
    Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
        attrCatBuffer.getRecord(attrCatRecord,i);
        AttrCacheEntry attrCacheEntry;
        AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&attrCacheEntry.attrCatEntry);
        attrCacheEntry.dirty=false;
        attrCacheEntry.recId.block=ATTRCAT_BLOCK;
        attrCacheEntry.recId.slot=i;
        attrCacheEntry.next=nullptr;
        AttrCacheEntry *attrCacheEntry2=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        *attrCacheEntry2=attrCacheEntry;
        attrCacheEntry2->next=AttrCacheTable::attrCache[RELCAT_RELID];
        AttrCacheTable::attrCache[RELCAT_RELID]=attrCacheEntry2;

    }
    else{
    RecBuffer attrCatBuffer(ATTRCAT_BLOCK);
    Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
        attrCatBuffer.getRecord(attrCatRecord,i);
        AttrCacheEntry attrCacheEntry;
        AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&attrCacheEntry.attrCatEntry);
        attrCacheEntry.dirty=false;
        attrCacheEntry.recId.block=ATTRCAT_BLOCK;
        attrCacheEntry.recId.slot=i;
        attrCacheEntry.next=nullptr;
        AttrCacheEntry *attrCacheEntry2=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        *attrCacheEntry2=attrCacheEntry;
        attrCacheEntry2->next=AttrCacheTable::attrCache[ATTRCAT_RELID];
        AttrCacheTable::attrCache[ATTRCAT_RELID]=attrCacheEntry2;

    }
   }

  // in the tableMetaInfo array
  //   set free = false for RELCAT_RELID and ATTRCAT_RELID
  //   set relname for RELCAT_RELID and ATTRCAT_RELID

  OpenRelTable::tableMetaInfo[RELCAT_RELID].free=false;
  OpenRelTable::tableMetaInfo[ATTRCAT_RELID].free=false;


}

OpenRelTable::~OpenRelTable()
 {

  for (int i = 2; i < MAX_OPEN; ++i) 
  {
    if (!tableMetaInfo[i].free) 
    {
      OpenRelTable::closeRel(i); // we will implement this function later
    }
  }

  // free the memory allocated for rel-id 0 and 1 in the caches
  free(RelCacheTable::relCache[0]);
  free(RelCacheTable::relCache[1]);
  
}




int OpenRelTable::getFreeOpenRelTableEntry() 
{

  /* traverse through the tableMetaInfo array,
    find a free entry in the Open Relation Table.*/
  for(int i=0;i<MAX_OPEN;i++)
  {
    if(tableMetaInfo[i].free==true)
    {
      return i;
    }
  }
  return E_CACHEFULL;

  // if found return the relation id, else return E_CACHEFULL.
}
int OpenRelTable::getRelId(char relName[ATTR_SIZE]) 
{

  /* traverse through the tableMetaInfo array,
    find the entry in the Open Relation Table corresponding to relName.*/

  // if found return the relation id, else indicate that the relation do not
  // have an entry in the Open Relation Table.

   for(int i=0;i<MAX_OPEN;i++)
   {
    if(tableMetaInfo[i].free==false)
    {
      if(strcmp(tableMetaInfo[i].relName,(char*)relName)==0)
      {
        return i;
      }
    }
   }
   return E_RELNOTOPEN;
}
int OpenRelTable::openRel(char relName[ATTR_SIZE]) 
{

  if(getRelId((char*)relName)!=E_RELNOTOPEN)
  {
    // (checked using OpenRelTable::getRelId())

    // return that relation id;
    return getRelId((char*)relName);
  }

  /* find a free slot in the Open Relation Table
     using OpenRelTable::getFreeOpenRelTableEntry(). */
  int relId=getFreeOpenRelTableEntry();


  if (relId == E_CACHEFULL)
  {
    return E_CACHEFULL;
  }

  // let relId be used to store the free slot.

  /****** Setting up Relation Cache entry for the relation ******/

  /* search for the entry with relation name, relName, in the Relation Catalog using
      BlockAccess::linearSearch().
      Care should be taken to reset the searchIndex of the relation RELCAT_RELID
      before calling linearSearch().*/

   RelCacheTable::resetSearchIndex(RELCAT_RELID);

  // relcatRecId stores the rec-id of the relation `relName` in the Relation Catalog.
    Attribute relNameAttribute;
    memcpy(relNameAttribute.sVal, relName, ATTR_SIZE);

  RecId relcatRecId=BlockAccess::linearSearch(RELCAT_RELID,(char*)RELCAT_ATTR_RELNAME,relNameAttribute,EQ);

  if(relcatRecId.block == -1 && relcatRecId.slot == -1) 
  {
    // (the relation is not found in the Relation Catalog.)
    return E_RELNOTEXIST;
  }

  /* read the record entry corresponding to relcatRecId and create a relCacheEntry
      on it using RecBuffer::getRecord() and RelCacheTable::recordToRelCatEntry().
      update the recId field of this Relation Cache entry to relcatRecId.
      use the Relation Cache entry to set the relId-th entry of the RelCacheTable.
    NOTE: make sure to allocate memory for the RelCacheEntry using malloc()
  */
  RelCacheEntry relCacheEntry;
  RecBuffer obj(relcatRecId.block);
  HeadInfo head;
  obj.getHeader(&head);
  Attribute record[RELCAT_NO_ATTRS];
  obj.getRecord(record,relcatRecId.slot);
  RelCacheTable::recordToRelCatEntry(record,&relCacheEntry.relCatEntry);
  relCacheEntry.recId=relcatRecId;
  RelCacheTable::relCache[relId]=(RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[relId])=relCacheEntry;



  /****** Setting up Attribute Cache entry for the relation ******/

  // let listHead be used to hold the head of the linked list of attrCache entries.
  AttrCacheEntry* listHead;

  /*iterate over all the entries in the Attribute Catalog corresponding to each
  attribute of the relation relName by multiple calls of BlockAccess::linearSearch()
  care should be taken to reset the searchIndex of the relation, ATTRCAT_RELID,
  corresponding to Attribute Catalog before the first call to linearSearch().*/
  RelCacheTable::resetSearchIndex(ATTRCAT_RELID);
  int i=0;
  AttrCacheEntry* prev=nullptr;
  AttrCacheEntry* curr=nullptr;
  while(i<relCacheEntry.relCatEntry.numAttrs)
  {
      /* let attrcatRecId store a valid record id an entry of the relation, relName,
      in the Attribute Catalog.*/
      RecId attrcatRecId=BlockAccess::linearSearch(ATTRCAT_RELID,(char*)ATTRCAT_ATTR_RELNAME,relNameAttribute,EQ);

      /* read the record entry corresponding to attrcatRecId and create an
      Attribute Cache entry on it using RecBuffer::getRecord() and
      AttrCacheTable::recordToAttrCatEntry().
      */
      RecBuffer obj2(attrcatRecId.block);
      HeadInfo head2;
      obj2.getHeader(&head2);
      Attribute record2[ATTRCAT_NO_ATTRS];
      obj2.getRecord(record2,attrcatRecId.slot);
      AttrCacheEntry attrCacheEntry;
      AttrCacheTable::recordToAttrCatEntry(record2,&attrCacheEntry.attrCatEntry);
      attrCacheEntry.recId=attrcatRecId;
      attrCacheEntry.next=nullptr;
      
      /*update the recId field of this Attribute Cache entry to attrcatRecId.
      add the Attribute Cache entry to the linked list of listHead .*/
      // NOTE: make sure to allocate memory for the AttrCacheEntry using malloc()
      if(prev==nullptr)
      {
        listHead=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        *listHead=attrCacheEntry;
        prev=listHead;
      }
      else
      {
        curr=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        *curr=attrCacheEntry;
        prev->next=curr;
        prev=curr;
      }
      i++;
  }

  // set the relIdth entry of the AttrCacheTable to listHead.
  AttrCacheTable::attrCache[relId]=listHead;

  /****** Setting up metadata in the Open Relation Table for the relation******/

  // update the relIdth entry of the tableMetaInfo with free as false and
  // relName as the input.
  tableMetaInfo[relId].free=false;
  strcpy(tableMetaInfo[relId].relName,(char*)relName);

  return relId;
}
int OpenRelTable::closeRel(int relId) 
{
  if (relId == RELCAT_RELID || relId == ATTRCAT_RELID) 
  {
    return E_NOTPERMITTED;
  }

  if (relId < 0 || relId >= MAX_OPEN) 
  {
    return E_OUTOFBOUND;
  }

  if (tableMetaInfo[relId].free) 
  {
    return E_RELNOTOPEN;
  }

  // free the memory allocated in the relation and attribute caches which was
  // allocated in the OpenRelTable::openRel() function


  // update `tableMetaInfo` to set `relId` as a free slot
  // update `relCache` and `attrCache` to set the entry at `relId` to nullptr
  free(RelCacheTable::relCache[relId]);
  free(AttrCacheTable::attrCache[relId]);

  
  tableMetaInfo[relId].free=true;
  RelCacheTable::relCache[relId] = nullptr;
  AttrCacheTable::attrCache[relId] = nullptr;


  return SUCCESS;
}
int OpenRelTable::closeRel(int relId) 
{
 if (relId == RELCAT_RELID || relId == ATTRCAT_RELID) 
  {
    return E_NOTPERMITTED;
  }

  if (relId < 0 || relId >= MAX_OPEN) 
  {
    return E_OUTOFBOUND;
  }

  if (tableMetaInfo[relId].free) 
  {
    return E_RELNOTOPEN;
  }
  RelCacheEntry *relCacheEntry = RelCacheTable::relCache[relId];

  if (RelCacheTable::relCache[relId]!=nullptr && RelCacheTable::relCache[relId]->dirty==true)
  {

    /* Get the Relation Catalog entry from RelCacheTable::relCache
    Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
    Attribute attr[RELCAT_NO_ATTRS];
    RelCatEntry relcat=RelCacheTable::relCache[relId]->relCatEntry;
    RelCacheTable::relCatEntryToRecord(&relcat,attr);
    RecId recId=RelCacheTable::relCache[relId]->recId;



    // declaring an object of RecBuffer class to write back to the buffer
    RecBuffer relCatBlock(recId.block);

    // Write back to the buffer using relCatBlock.setRecord() with recId.slot
    relCatBlock.setRecord(attr,recId.slot);

  }

  /****** Releasing the Attribute Cache entry of the relation ******/
  free(relCacheEntry);
    RelCacheTable::relCache[relId] = nullptr;

  // free the memory allocated in the attribute caches which was
  // allocated in the OpenRelTable::openRel() function
  free(AttrCacheTable::attrCache[relId]);


  // (because we are not modifying the attribute cache at this stage,
  // write-back is not required. We will do it in subsequent
  // stages when it becomes needed)



  /****** Set the Open Relation Table entry of the relation as free ******/
  AttrCacheTable::attrCache[relId] = nullptr;

    tableMetaInfo[relId].free = true;


  // update `metainfo` to set `relId` as a free slot

  return SUCCESS;
}
