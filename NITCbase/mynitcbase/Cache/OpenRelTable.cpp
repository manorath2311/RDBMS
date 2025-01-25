#include "OpenRelTable.h"

#include <cstring>
#include <cstdlib>
OpenRelTable::OpenRelTable()
{
    for(int i=0;i<MAX_OPEN;i++)
    {
        RelCacheTable::relCache[i]=nullptr;
        AttrCacheTable::attrCache[i]=nullptr;
    }
    //RelCache
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
  int j=2,bna,sna;
    RecBuffer relCatBuffer(RELCAT_BLOCK);
    HeadInfo relCatHeader;
    relCatBuffer.getHeader(&relCatHeader);
    for(int i=0;i<relCatHeader.numEntries;i++)
    {
        Attribute relCatRecord[RELCAT_NO_ATTRS];
        relCatBuffer.getRecord(relCatRecord,i);
    if(strcmp(relCatRecord[RELCAT_REL_NAME_INDEX].sVal,"Students")==0)
    {
        RelCacheEntry relCacheEntry;
        RelCacheTable::recordToRelCatEntry(relCatRecord,&relCacheEntry.relCatEntry);
       relCacheEntry.dirty=false;
        relCacheEntry.recId.block=RELCAT_BLOCK;
        relCacheEntry.recId.slot=i;
        RelCacheTable::relCache[j]=(RelCacheEntry*)malloc(sizeof(RelCacheEntry));
        *(RelCacheTable::relCache[j])=relCacheEntry;
    break;
    }
    }
    RecBuffer attrCatBuffer(ATTRCAT_BLOCK);
    HeadInfo attrCatHeader;
    attrCatBuffer.getHeader(&attrCatHeader);
    int bn=ATTRCAT_BLOCK;
    for(int i=0;i<attrCatHeader.numEntries;i++)
    {
        Attribute attrCatRecord[RELCAT_NO_ATTRS];
        attrCatBuffer.getRecord(attrCatRecord,i);
        if(strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,"Students")==0)
        {
            AttrCacheEntry attrCacheEntry;
        AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&attrCacheEntry.attrCatEntry);
        attrCacheEntry.dirty=false;
        attrCacheEntry.recId.block=bn;
        attrCacheEntry.recId.slot=i;
        attrCacheEntry.next=nullptr;
        AttrCacheEntry *attrCacheEntry2=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        *attrCacheEntry2=attrCacheEntry;
        attrCacheEntry2->next=AttrCacheTable::attrCache[j];
        AttrCacheTable::attrCache[j]=attrCacheEntry2;
        }
        if(i==19&&attrCatHeader.numEntries==20)
        {
            RecBuffer attrCatBuffer2(attrCatHeader.rblock);
            HeadInfo attrCatHeader2;
            attrCatBuffer2.getHeader(&attrCatHeader2);
            if(attrCatHeader2.numEntries!=0)
            {
                i=-1;
                bn=attrCatHeader.rblock;
                attrCatBuffer=attrCatBuffer2;
                attrCatHeader=attrCatHeader2;
            }

        }
    }

}
OpenRelTable::~OpenRelTable()
{
    for(int i=0;i<MAX_OPEN;i++)
    {
        if(RelCacheTable::relCache[i]!=nullptr)
    delete(RelCacheTable::relCache[i]);
    }
    for(int i=0;i<MAX_OPEN;i++)
    {
        AttrCacheEntry*next;
       for(AttrCacheEntry*attrCacheEntry= AttrCacheTable::attrCache[i];attrCacheEntry!=nullptr;attrCacheEntry=next)
       {
           next=attrCacheEntry->next;
           delete(attrCacheEntry);
       }
    }
}
int OpenRelTable::getRelId(char relName[ATTR_SIZE])
{
    if(strcmp(relName,RELCAT_RELNAME)==0)
    {
        return RELCAT_RELID;
    }
    if(strcmp(relName,ATTRCAT_RELNAME)==0)
    {
        return ATTRCAT_RELID;
    }
    if(strcmp(relName,"Students")==0)
    {
        return 2;
    }
    return E_RELNOTOPEN;
}
