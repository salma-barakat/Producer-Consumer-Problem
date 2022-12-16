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
#include <time.h>
#include <queue>
using namespace std; 

struct producer{
	char name[10];//1 byte
	double currentPrice = 0;//8 byte
	int index;//4yte	
	double avgPrice = 0; //8 byte 
	queue<double> prevPrices;
	double prevOne = 0;	 
	double prevAvg = 0;
};
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
int intervalsec,r,N;
double mean,deviation,pricee;
char name[10];
struct timespec ts;
int main(int argc,char *argv[]){
	strcpy(name,argv[1]);
	mean=stod(argv[2]);
	deviation=stod(argv[3]);
	intervalsec=stoi(argv[4]);
	N=stoi(argv[5]);
	default_random_engine generator;
	normal_distribution<double>distribution(mean,pow(deviation,2));
	struct sembuf sem_buf;
	union semun sem_val;
	key_t key0=10;
	key_t key1=11;
	key_t key2=12;
	key_t key3=13;
	int semid0=semget(key0,1,IPC_CREAT | IPC_EXCL |0666); //s
	int semid1=semget(key1,1,IPC_CREAT | IPC_EXCL |0666); //n
	int semid2=semget(key2,1,IPC_CREAT | IPC_EXCL |0666); //e
	if(semid0>=0 and semid1>=0 and semid2>=0){
	sem_val.val=1;
	int x0=semctl(semid0,0,SETVAL,sem_val);
	sem_val.val=0;
	x0=semctl(semid1,0,SETVAL,sem_val);
	sem_val.val=N;
	x0=semctl(semid2,0,SETVAL,sem_val);
	}else{
	if(semid0==-1 and errno==EEXIST ){
		semid0=semget(key0,1,0); 	
	}
	if(semid1==-1 and errno==EEXIST ){
		semid1=semget(key1,1,0); 	
	}
	if(semid2==-1 and errno==EEXIST ){
		semid2=semget(key2,1,0); 	
	}
	}
	int shmid =shmget(key3,N*sizeof(producer),0666 | IPC_CREAT);
	producer *buffer=(producer *)shmat(shmid,0,0);
	while(true){
		pricee=distribution(generator);
		 //generating a new value:
    		auto now = std::chrono::high_resolution_clock::now();
    		auto timee = std::chrono::system_clock::to_time_t(now);
    		auto tm = *std::localtime(&timee);
		clock_gettime(CLOCK_REALTIME,&ts);
		cout <<"["<<put_time(&tm, "%m/%d/%Y %T.")<<ts.tv_nsec<<"] "<<name<<": Generating a new value "<<pricee<<endl;
		//sem_op -ve,block until enough resources -> wait
		//sem_op +ve,realse resource-> signal
		now = std::chrono::high_resolution_clock::now();
    		timee = std::chrono::system_clock::to_time_t(now);
    		tm = *std::localtime(&timee);
		clock_gettime(CLOCK_REALTIME,&ts);
		cout <<"["<<put_time(&tm, "%m/%d/%Y %T.")<<ts.tv_nsec<<"] "<<name<<": trying to get mutex on shared buffer "<<endl;
		
		sem_buf.sem_op=-1;	//wait for e -> number of empty cells
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r= semop(semid2,&sem_buf,1);
		
		
		sem_buf.sem_op=-1;	//wait for s
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid0,&sem_buf,1);
		//cs
		now = std::chrono::high_resolution_clock::now();
    		timee = std::chrono::system_clock::to_time_t(now);
    		tm = *std::localtime(&timee);
		clock_gettime(CLOCK_REALTIME,&ts);
		cout <<"["<<put_time(&tm, "%m/%d/%Y %T.")<<ts.tv_nsec<<"] "<<name<<": placing "<<pricee<<" on shared buffer"<<endl;
		
		strcpy(buffer[buffer->index].name,name);
		buffer[buffer->index].currentPrice=pricee;
		buffer->index=(buffer->index+1)%N;
		//...
		sem_buf.sem_op=1;	//signal for s
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid0,&sem_buf,1);
		sem_buf.sem_op=1;	//signal for n
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid1,&sem_buf,1);
		
		now = std::chrono::high_resolution_clock::now();
    		timee = std::chrono::system_clock::to_time_t(now);
    		tm = *std::localtime(&timee);
		clock_gettime(CLOCK_REALTIME,&ts);
		cout <<"["<<put_time(&tm, "%m/%d/%Y %T.")<<ts.tv_nsec<<"] "<<name<<": sleeping for "<<intervalsec<<"ms"<<endl;
		sleep(intervalsec/1000);
		cout<<"------------------------------"<<endl;
	}
	return 0;
}