#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
using namespace std;

struct buff{
char* name;
float priceMean;
float standardDev;
int sleepInterval;
};

int main()
{
  int i;
  void *shared_memory;
  char buff[100];
    // ftok to generate unique key
    key_t key = ftok("shmfile",65);
//msqid = msgget(ftok("/tmp", 'A'), (IPC_CREAT | IPC_EXCL | 0400));
  
    // shmget returns an identifier in shmid
    // shmget used to create the shared memory segment
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    cout<<"Key of shared memory is "<<shmid<<endl;

// shmat to attach the shared segment with the address space of the process
    shared_memory=shmat(shmid,NULL,0);
    cout<<"Process attached at "<<shared_memory<<endl;

    read(0,buff, 100); //get input from user
    strcpy((char *)shared_memory,buff); //data written to shared memory
    cout<<"You wrote: "<<(char *)shared_memory<<endl;
    //printf("You wrote : %s\n",(char *)shared_memory);
      
    //detach from shared memory 
    shmdt(shared_memory);
  
    return 0;
}