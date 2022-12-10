#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct buff{
string name;
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

    // shmget returns an identifier in shmid
    // shmget used to create the shared memory segment
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    cout<<"Key of shared memory is "<<shmid<<endl;

    // shmat to attach the shared segment with the address space of the process
    shared_memory=shmat(shmid,NULL,0);
    cout<<"Process attached at "<<shared_memory<<endl;
    struct buff b;
    char* buf = (char *)shared_memory;
    stringstream ss(buf); //used for tokenization
    int k = 0;
    while (ss.good()){
      string substr;
      getline(ss, substr, ' ');
      if (k == 0){
        b.name = substr;
        k++;
      }
      else if (k == 1){
        b.priceMean = stof(substr);
        k++;
      }
      else if (k == 2){
        b.standardDev = stof(substr);
        k++;
      }
      else if (k == 3){
        b.sleepInterval = stoi(substr);
        k++;
      }
    }

  cout<<"commodity name: "<<b.name<<endl;
  cout<<"commodity mean: "<<b.priceMean<<endl;
  cout<<"commodity stnddev "<<b.standardDev<<endl;
  cout<<"sleep interval: "<<b.sleepInterval<<endl;



   // printf("Data read from shared memory is : %s\n",(char *)shared_memory);

    //detach from shared memory
    shmdt(shared_memory);

    return 0;
}