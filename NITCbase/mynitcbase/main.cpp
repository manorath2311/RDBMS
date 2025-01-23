#include <cstring>
#include <iostream>
#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
//#define BLOCK_SIZE 2048;
using namespace std;
int main(int argc, char *argv[])
{
  /*//STAGE_1
  Disk disk_run;

  unsigned char buffer[BLOCK_SIZE];
  Disk::readBlock(buffer, 7000);
  char message[] = "hello";
  memcpy(buffer+20, message, 6);
  Disk::writeBlock(buffer, 7000);

  unsigned char buffer2[BLOCK_SIZE];
  char message2[6];
  Disk::readBlock(buffer2, 7000);
  memcpy(message2, buffer2+20, 6);
  std::cout << message2<<"\n";


  unsigned char buffer3[BLOCK_SIZE];
  int j;
  unsigned char msg3[25];
  Disk::readBlock(buffer3,0);
  memcpy(msg3,buffer3,20);
  for(int i=0;i<20;i++)
  {
    j=(int)msg3[i];
    cout<<j<<" ";
  }
  cout<<"\n";
  //STAGE_1_ENDS
*/


   //STAGE_2
  /*Disk disk_run;
  StaticBuffer buffer;


  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatheader;
  HeadInfo attrCatheader;
  HeadInfo attrCatheader2;
  relCatBuffer.getHeader(&relCatheader);
  attrCatBuffer.getHeader(&attrCatheader);
  int x;
  unsigned char bufferm[BLOCK_SIZE];
  unsigned char b[16]="Batch";
  for(int i=0;i<relCatheader.numEntries;i++)
  {
    Attribute relCatRecord[RELCAT_NO_ATTRS];
    relCatBuffer.getRecord(relCatRecord,i);
    printf("Relation : %s\n",relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    int j=0;
    RecBuffer attrCatBuffer(ATTRCAT_BLOCK);
    attrCatBuffer.getHeader(&attrCatheader);
    x=ATTRCAT_BLOCK;
    while(j<attrCatheader.numEntries)
    {
       Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
       attrCatBuffer.getRecord(attrCatRecord,j);
       if(strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal)==0)
       {
         const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";

          if(strcmp(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal , "Class")==0)
          {
            Disk::readBlock(bufferm,x);
            memcpy(bufferm+52+j*96+16,b,16);
            Disk::writeBlock(bufferm,x);
            attrCatBuffer.getRecord(attrCatRecord,j);
          }

          printf(" %s: %s ",attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,attrType);
          printf("%d %d\n",x,j);
       }
      if(j==attrCatheader.numEntries-1 && (int)attrCatheader.rblock!=-1)
       {

            x=(int)attrCatheader.rblock;
            RecBuffer attrCatBuffer2(x);
            attrCatBuffer2.getHeader(&attrCatheader2);
            attrCatBuffer=attrCatBuffer2;
            attrCatheader=attrCatheader2;
            j=-1;

       }

      j++;
    }

  }
  //STAGE_2_ENDS
  */


  //STAGE_3
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  /*for(int i=0;i<=2;i++)
  {
    RelCatEntry relCat;
    RelCacheTable::getRelCatEntry(i,&relCat);
    printf("Relation: %s\n",relCat.relName);
    int x=relCat.numAttrs;
    for(int j=0;j<x;j++)
    {
      AttrCatEntry attrCatBuffer;
      AttrCacheTable::getAttrCatEntry(i,j,&attrCatBuffer);
      char type[4];
      strcpy(type,(attrCatBuffer.attrType==1)? "STR" : "NUM");
      printf("  %s :%s\n",attrCatBuffer.attrName,type);
    }
    printf("\n");
  }
  */
  struct RelCatEntry rce;
  struct AttrCatEntry ace;

  for(int i=0;i<3;i++)
  {
    RelCacheTable::getRelCatEntry(i,&rce);
    printf("Relation: %s\n",rce.relName);
    int x=rce.numAttrs;
    for(int j=0;j<x;j++)
    {
      AttrCacheTable::getAttrCatEntry(i,j,&ace);
      char type[4];
      strcpy(type,(ace.attrType==1)? "STR" : "NUM");
      printf(" %s %s\n",ace.attrName,type);
    }
    printf("\n");
  }

  AttrCacheEntry* x=AttrCacheTable::attrCache[1];
  char str[10];
  while(x!=NULL)
  {
    strcpy(str,x->attrCatEntry.attrName);
    strcpy(str,x->attrCatEntry.relName);
    printf(" %s ",str);
    printf("%d %d\n",x->recId.block,x->recId.slot);
    x=x->next;
  }
  


  return FrontendInterface::handleFrontend(argc, argv);




  return 0;
}




















