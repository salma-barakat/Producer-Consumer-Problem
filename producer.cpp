#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std; 

struct producer{
	char name[10];
	double price;
	int index=0;	 
};
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
#define size 5
int intervalsec,r;
double mean,deviation,pricee;
char name[10];
int main(int argc,char *argv[]){
	strcpy(name,argv[1]);
	mean=stod(argv[2]);
	deviation=stod(argv[3]);
	intervalsec=stoi(argv[4]);
	default_random_engine generator;
	normal_distribution<double>distribution(mean,pow(deviation,2));
	struct sembuf sem_buf;
	union semun sem_val;
	key_t key0=ftok("test",10);
	key_t key1=ftok("test",11);
	key_t key2=ftok("test",12);
	key_t key3=ftok("test",13);
	int semid0=semget(key0,1,IPC_EXCL |0666); //s
	int semid1=semget(key1,1,IPC_EXCL |0666); //n
	int semid2=semget(key2,1,IPC_EXCL |0666); //e
	//sem_val.val=1;
	//int x0=semctl(semid0,0,SETVAL,sem_val);
	//sem_val.val=0;
	//x0=semctl(semid1,0,SETVAL,sem_val);
	//sem_val.val=size;
	//x0=semctl(semid2,0,SETVAL,sem_val);
	int shmid =shmget(key3,size,0666|IPC_CREAT);
    //generating a new value:
    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::gmtime(&time);
    auto epoch = now.time_since_epoch();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(epoch).count() % 1000000;

    cout << std::put_time(&tm, "%F %T.") << us << std::put_time(&tm, " %Z") << name<<
    "Generating a new value "<<shmid<<endl;



	producer *buffer=(producer *)shmat(shmid,0,0);
	//buffer[size];
	if(semid0==-1 and errno==EEXIST ){
		semid0=semget(key0,1,0); 	
	}
	if(semid1==-1 and errno==EEXIST ){
		semid1=semget(key1,1,0); 	
	}
	if(semid2==-1 and errno==EEXIST ){
		semid2=semget(key2,1,0); 	
	}
	while(true){
		cout<<"e:"<<semctl(semid2,0,GETVAL,sem_val)<<endl;;
		cout<<semid0<<endl;
		cout<<semid1<<endl;
		cout<<semid2<<endl;
		pricee=distribution(generator);
		//sem_op -ve,block until enough resources -> wait
		//sem_op +ve,realse resource-> signal
		sem_buf.sem_op=-1;	//wait for e -> number of empty cells
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r= semop(semid2,&sem_buf,1);
		sem_buf.sem_op=-1;	//wait for s
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid0,&sem_buf,1);
		//cs
		strcpy(buffer[buffer->index].name,name);
		buffer[buffer->index].price=pricee;
		cout<<buffer[buffer->index].name<<endl;
		cout<<buffer[buffer->index].price<<endl;
		cout<<buffer->index<<endl;
		buffer->index=(buffer->index+1)%size;
		//...
		sem_buf.sem_op=1;	//signal for s
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid0,&sem_buf,1);
		sem_buf.sem_op=1;	//signal for n
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid1,&sem_buf,1);
		cout<<"....."<<endl;
        auto now = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::gmtime(&time);
        auto epoch = now.time_since_epoch();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(epoch).count() % 1000000;

        cout << std::put_time(&tm, "%F %T.") << us << std::put_time(&tm, " %Z") <<
        "Sleeping for "<<intervalsec <<" ms"<<endl;
		sleep(intervalsec/1000);
	}
	return 0;
}