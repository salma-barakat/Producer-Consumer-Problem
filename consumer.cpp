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
#include <iterator>
#include <queue>
using namespace std; 
struct consumer{
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

char name1[10];
double price;
#define Size 5
int r,i=0, N;
int main(int argc, char *argv[]){
	N=stoi(argv[1]);
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
	sem_val.val=Size;
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
	int shmid =shmget(key3,N*sizeof(consumer),0666 | IPC_CREAT);
	consumer *buffer=(consumer *)shmat(shmid,0,0);
	struct consumer commodities[11];
	strcpy(commodities[0].name,"ALUMINIUM");
	strcpy(commodities[1].name,"COPPER");
	strcpy(commodities[2].name,"COTTON");
	strcpy(commodities[3].name,"CRUDEOIL");
	strcpy(commodities[4].name,"GOLD	");
	strcpy(commodities[5].name,"LEAD	");
	strcpy(commodities[6].name,"MENTHAOIL");
	strcpy(commodities[7].name,"NATURALGAS");
	strcpy(commodities[8].name,"NICKEL");
	strcpy(commodities[9].name,"SILVER");
	strcpy(commodities[10].name,"ZINC	");
	for(int i=0; i<11; i++){
		for(int j=0; j<5; j++){
			commodities[i].prevPrices.push(0);
		}
	}
	consumer c;
	int comm;
	while(true){
		sem_buf.sem_op=-1;	//wait for n 
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r= semop(semid1,&sem_buf,1);
		sem_buf.sem_op=-1;	//wait for s
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid0,&sem_buf,1);
		//cs..
		strcpy(c.name,buffer[i].name);
		c.currentPrice=buffer[i].currentPrice;
		if(strcasecmp(c.name, "ALUMINIUM")==0)
			comm = 0;
		else if(strcmp(c.name, "COPPER")==0)
			comm = 1;
		else if(strcmp(c.name, "COTTON")==0)
			comm = 2;
		else if(strcmp(c.name, "CRUDEOIL")==0)
			comm = 3;
		else if(strcmp(c.name, "GOLD")==0)
			comm = 4;
		else if(strcmp(c.name, "LEAD")==0)
			comm = 5;
		else if(strcmp(c.name, "MENTHAOIL")==0)
			comm = 6;
		else if(strcmp(c.name, "NATURALGAS")==0)
			comm = 7;
		else if(strcmp(c.name, "NICKEL")==0)
			comm = 8;
		else if(strcmp(c.name, "SILVER")==0)
			comm = 9;
		else if(strcmp(c.name, "ZINC")==0)
			comm = 10;
		if(commodities[comm].prevPrices.size()==5){
			commodities[comm].prevPrices.pop();
		}

		commodities[comm].currentPrice = c.currentPrice;
		commodities[comm].prevPrices.push(c.currentPrice);
		i=(i+1)%N;

	 
		//..
		sem_buf.sem_op=1;	//signal for s
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid0,&sem_buf,1);
		sem_buf.sem_op=1;	//signal for e
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid2,&sem_buf,1);
		sleep(3);
		commodities[comm].avgPrice = 0;
		queue<double> temp = commodities[comm].prevPrices;
		while (!temp.empty()) {
			commodities[comm].avgPrice += temp.front();
			temp.pop();
		}
		commodities[comm].avgPrice /= 5;
		printf("\e[1;1H\e[2J");
		cout<<"+-------------------------------------+"<<endl;
		cout<<"| Currency	|  Price   | AvgPrice |"<<endl;
		cout<<"+-------------------------------------+"<<endl;

		for(int k=0; k<11; k++){
			cout<<"| "<<commodities[k].name<<"	|";

			  
			if(commodities[k].currentPrice>commodities[k].prevOne){
				printf("\033[1;32m");
				printf("%7.2lf↑  ", commodities[k].currentPrice);
				printf("\033[0m|");
			}
				
			else if(commodities[k].currentPrice<commodities[k].prevOne){
				printf("\033[1;31m");
				printf("%7.2lf↓  ", commodities[k].currentPrice);
				printf("\033[0m|");
			}
				
			else{
				printf("\033[1;34m");
				printf("%7.2lf-  ", commodities[k].currentPrice);
				printf("\033[0m|");
			}

			if(commodities[k].avgPrice>commodities[k].prevAvg){
				printf("\033[1;32m");
				printf("%7.2lf↑  ", commodities[k].avgPrice);
				printf("\033[0m|");
			}
				
			else if(commodities[k].avgPrice<commodities[k].prevAvg){
				printf("\033[1;31m");
				printf("%7.2lf↓  ", commodities[k].avgPrice);
				printf("\033[0m|");
			}
				
			else{
				printf("\033[1;34m");
				printf("%7.2lf-  ", commodities[k].avgPrice);
				printf("\033[0m|");
			}
			cout<<endl;
				
			
			
		}
		cout<<"+-------------------------------------+"<<endl;
		
	  commodities[comm].prevOne = commodities[comm].currentPrice;
	  commodities[comm].prevAvg = commodities[comm].avgPrice;

		 
		
	}

}