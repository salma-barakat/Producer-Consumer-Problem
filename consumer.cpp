#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <iostream>
using namespace std;


int main()
{
  int i;
  void *shared_memory;
  char buff[100];
    // ftok to generate unique key
    key_t key = ftok("shmfile",65);

    // shmget returns an identifier in shmid
    // shmget used to create the shared memory segment
    int shmid = shmget(key,1024,0666);
    cout<<"Key of shared memory is "<<shmid<<endl;

    // shmat to attach the shared segment with the address space of the process
    shared_memory=shmat(shmid,NULL,0);
    cout<<"Process attached at "<<shared_memory<<endl;
    printf("Data read from shared memory is : %s\n",(char *)shared_memory);

    //detach from shared memory
    //shmdt(str);

    return 0;
}
