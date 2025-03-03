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
    else
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

// OpenRelTable::OpenRelTable()
// {

//   // initialize relCache and attrCache with nullptr
//   for (int i = 0; i < MAX_OPEN;i++)
//   {
//     RelCacheTable::relCache[i] = nullptr;
//     AttrCacheTable::attrCache[i] = nullptr;
//   }
    //  for(int i=0;i<MAX_OPEN;i++)
    // {
    //   OpenRelTable::tableMetaInfo[i].free=true;
    // }

//   /************ Setting up Relation Cache entries ************/
//   // (we need to populate relation cache with entries for the relation catalog
//   //  and attribute catalog.)

//   /**** setting up Relation Catalog relation in the Relation Cache Table****/
//   RecBuffer relCatBlock(RELCAT_BLOCK);

//   Attribute relCatRecord[RELCAT_NO_ATTRS];
//   relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

//   struct RelCacheEntry relCacheEntry;
//   RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
//   relCacheEntry.recId.block = RELCAT_BLOCK;
//   relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

//   // allocate this on the heap because we want it to persist outside this function
//   RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;

//   /**** setting up Attribute Catalog relation in the Relation Cache Table ****/

//   // set up the relation cache entry for the attribute catalog similarly
//   // from the record at RELCAT_SLOTNUM_FOR_ATTRCAT

//    relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_ATTRCAT);
//    //struct RelCacheEntry relCacheEntry;
//   RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
//   relCacheEntry.recId.block = RELCAT_BLOCK;
//   relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_ATTRCAT;

//   // allocate this on the heap because we want it to persist outside this function
//   RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[ATTRCAT_RELID]) = relCacheEntry;

//   // set the value at RelCacheTable::relCache[ATTRCAT_RELID]


//   /************ Setting up Attribute cache entries ************/
//   // (we need to populate attribute cache with entries for the relation catalog
//   //  and attribute catalog.)

//   /**** setting up Relation Catalog relation in the Attribute Cache Table ****/

//   RecBuffer obj1(RELCAT_BLOCK);
//    Attribute at[4];
//   obj1.getRecord(at,2);
//   struct RelCacheEntry RR1;
//   struct RelCatEntry RR2;

//   RelCacheTable::recordToRelCatEntry(at,&(RR1.relCatEntry));
// RR1.recId.block=RELCAT_BLOCK;
// RR1.recId.slot=2;
// RelCacheTable::relCache[2] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[2]) = RR1;






// AttrCacheEntry* relhead=nullptr;
// AttrCacheEntry* relprev=nullptr;
// RecBuffer relcat(ATTRCAT_BLOCK);
// Attribute relattr[6];

// for(int i=0;i<6;i++)
// {
//   AttrCacheEntry* x=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//   relcat.getRecord(relattr,i);
//   AttrCacheTable::recordToAttrCatEntry(relattr,&(x->attrCatEntry));
//   x->recId.slot=i;
//   x->recId.block=RELCAT_BLOCK;
//   x->next=nullptr;
//   if(relprev==nullptr)
//   {
//     relhead=x;
//   }
//   else
//   {
//     relprev->next=x;
//   }
//   relprev=x;
// }




// AttrCacheTable::attrCache[RELCAT_RELID]=relhead;























// /* head of the linked list */;

//   /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/

//   // set up the attributes of the attribute cache similarly.
//   // read slots 6-11 from attrCatBlock and initialise recId appropriately

//   // set the value at AttrCacheTable::attrCache[ATTRCAT_RELID]
//     /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/

//   // Initialize the attribute cache for ATTRCAT_RELID
//   AttrCacheEntry* attrHead = nullptr;
//   AttrCacheEntry* attrPrev = nullptr;


//   RecBuffer attrCatBlock2(ATTRCAT_BLOCK);

//   Attribute attrRecord1[ATTRCAT_NO_ATTRS];

//   for (int i = RELCAT_NO_ATTRS; i <RELCAT_NO_ATTRS+ATTRCAT_NO_ATTRS;i++)
//   {
//     attrCatBlock2.getRecord(attrRecord1,i);

//     AttrCacheEntry* current = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     //memset(current, 0, sizeof(AttrCacheEntry));
//     AttrCacheTable::recordToAttrCatEntry(attrRecord1,&current->attrCatEntry);
//     current->recId.block = ATTRCAT_BLOCK;
//     current->recId.slot = i;
//     current->next = nullptr;

//     if (attrPrev)
//     {
//       attrPrev->next = current;
//     }
//     else
//     {
//       attrHead = current;
//     }
//     attrPrev = current;



//   }



//   AttrCacheTable::attrCache[ATTRCAT_RELID] = attrHead;
//   OpenRelTable::tableMetaInfo[RELCAT_RELID].free=false;
//   OpenRelTable::tableMetaInfo[ATTRCAT_RELID].free=false;





// /*

//   //RecBuffer attrCatBlock(ATTRCAT_BLOCK);
//   //Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
//   RecBuffer RelCatBuffer(RELCAT_BLOCK);
//   HeadInfo RelCatHeader;
//   RelCatBuffer.getHeader(&RelCatHeader);
//   int slot=-1;
//   int entries=-1;
//   int block=RELCAT_BLOCK;
//   for(int i=0;i<RelCatHeader.numEntries;i++)
//   {
//     Attribute RelCatRecord[RELCAT_NO_ATTRS];
//     RelCatBuffer.getRecord(RelCatRecord,i);
//     if(strcmp(RelCatRecord[RELCAT_REL_NAME_INDEX].sVal,"Students")==0)
//     {
//       slot=i;
//       entries=RelCatRecord[RELCAT_NO_ATTRS].nVal;
//       break;
//     }
//   }
//   struct RelCacheEntry cac1;
//   Attribute record[RELCAT_NO_ATTRS];
//   relCatBlock.getRecord(record,slot);
//   RelCacheTable::recordToRelCatEntry(record,&cac1.relCatEntry);
//   cac1.recId.block=RELCAT_BLOCK;
//   cac1.recId.slot=slot;
//   RelCacheTable::relCache[2]=(struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[2])=cac1;


// */



// }
void freeLinkedList(AttrCacheEntry *head)
{
    for (AttrCacheEntry *it = head, *next; it != nullptr; it = next)
    {
        next = it->next;
        free(it);
    }
}

OpenRelTable::~OpenRelTable() 
{

    for(int i=2;i<MAX_OPEN;i++)
    {
        if(tableMetaInfo[i].free==false)
        {
            // close the relation using openRelTable::closeRel().
            OpenRelTable::closeRel(i);
        }
    }
    // free the memory allocated for the relation catalog and the attribute catalog
    //do it it is still pending
    for(int i=2;i<MAX_OPEN;i++)
    {
        free(RelCacheTable::relCache[i]);
        freeLinkedList(AttrCacheTable::attrCache[i]);
        RelCacheTable::relCache[i]=nullptr;
        AttrCacheTable::attrCache[i]=nullptr;
    }

    /**** Closing the catalog relations in the relation cache ****/

    //releasing the relation cache entry of the attribute catalog

    if (RelCacheTable::relCache[ATTRCAT_RELID]->dirty)
    {

        /* Get the Relation Catalog entry from RelCacheTable::relCache
        Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
        Attribute attr[RELCAT_NO_ATTRS];
        RelCatEntry relcat=RelCacheTable::relCache[ATTRCAT_RELID]->relCatEntry;
        RelCacheTable::relCatEntryToRecord(&relcat,attr);
        RecId recId = RelCacheTable::relCache[ATTRCAT_RELID]->recId;

        // declaring an object of RecBuffer class to write back to the buffer

        RecBuffer relCatBlock(recId.block);

        // Write back to the buffer using relCatBlock.setRecord() with recId.slot
        relCatBlock.setRecord(attr,recId.slot);
        free(RelCacheTable::relCache[ATTRCAT_RELID]);

    }
    // free the memory dynamically allocated to this RelCacheEntry


    //releasing the relation cache entry of the relation catalog

    if(RelCacheTable::relCache[RELCAT_RELID]->dirty) 
    {

        Attribute relCatRecord[RELCAT_NO_ATTRS];

        RelCatEntry relCatEntry = RelCacheTable::relCache[RELCAT_RELID]->relCatEntry;
        RecId recId = RelCacheTable::relCache[RELCAT_RELID]->recId;

        RelCacheTable::relCatEntryToRecord(&relCatEntry, relCatRecord);

        RecBuffer relCatBlock(recId.block);
        relCatBlock.setRecord(relCatRecord, recId.slot);

        free(RelCacheTable::relCache[RELCAT_RELID]);
    }
    // free the memory dynamically allocated for this RelCacheEntry


    // free the memory allocated for the attribute cache entries of the
    // relation catalog and the attribute catalog
    freeLinkedList(AttrCacheTable::attrCache[RELCAT_RELID]);
    freeLinkedList(AttrCacheTable::attrCache[ATTRCAT_RELID]);
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
  if(strcmp(relName,"RELATIONCAT")==0)
  {
    return RELCAT_RELID;
  }
  if(strcmp(relName,"ATTRIBUTECAT")==0)
  {
    return ATTRCAT_RELID;
  }

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

  /****** Releasing the Relation Cache entry of the relation ******/

  if (RelCacheTable::relCache[relId]!=nullptr && RelCacheTable::relCache[relId]->dirty==1)
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

  // free the memory dynamically alloted to this Relation Cache entry
  // and assign nullptr to that entry
  free(RelCacheTable::relCache[relId]);
  RelCacheTable::relCache[relId] = nullptr;

  /****** Releasing the Attribute Cache entry of the relation ******/

  // for all the entries in the linked list of the relIdth Attribute Cache entry.
  for(AttrCacheEntry* it = AttrCacheTable::attrCache[relId]; it != nullptr; it = it->next)
  {
      if(it && it->dirty==1)
      {
          /* Get the Attribute Catalog entry from attrCache
           Then convert it to a record using AttrCacheTable::attrCatEntryToRecord().
           Write back that entry by instantiating RecBuffer class. Use recId
           member field and recBuffer.setRecord() */
          Attribute attr[ATTRCAT_NO_ATTRS];
          AttrCatEntry attrcat=it->attrCatEntry;
          AttrCacheTable::attrCatEntryToRecord(&attrcat,attr);
          RecId recId=it->recId;
          RecBuffer attrCatBlock(recId.block);
          attrCatBlock.setRecord(attr,recId.slot);

      }

      // free the memory dynamically alloted to this entry in Attribute
      // Cache linked list and assign nullptr to that entry
      free(it);

  }

  /****** Updating metadata in the Open Relation Table of the relation  ******/

  //free the relIdth entry of the tableMetaInfo.
  tableMetaInfo[relId].free = true;
  AttrCacheTable::attrCache[relId] = nullptr;

  return SUCCESS;
}