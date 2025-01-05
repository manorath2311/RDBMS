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
  //STAGE 1
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
  //STAGE 1 ENDS
  
 

  return 0;
}
  












