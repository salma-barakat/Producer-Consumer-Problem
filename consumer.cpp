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
	double currentPrice;//8 byte
	int index;//4yte	
	double avgPrice = 0; //8 byte 
	queue<double> prevPrices;
};
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
/*struct compare{
	bool operator()(process const& p1,process const& p2){
	
	if(p1.remainingtime==p2.remainingtime)
	return  p1.arrivaltime > p2.arrivaltime;
	else
	return p1.remainingtime>p2.remainingtime;
	} 
};*/
char name1[10];
double price;
#define Size 5
int r,i=0;
int main(){
	struct sembuf sem_buf;
	union semun sem_val;
	//priority_queue<struct process,vector <process>,compare> temp3;
	key_t key0=ftok("test",10);
	key_t key1=ftok("test",11);
	key_t key2=ftok("test",12);
	key_t key3=ftok("test",13);
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
	int shmid =shmget(key3,Size,0666 | IPC_CREAT);
	consumer *buffer=(consumer *)shmat(shmid,0,0);
	//vector<struct consumer> commodities;
	struct consumer commodities[11];
	consumer c;
	int comm;
	//int less4 = 1;
	char *arrow;
	//buffer[size];
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
		cout<<"line 91"<<endl;
		strcpy(c.name,buffer[i].name);
		c.currentPrice=buffer[i].currentPrice;
		cout<<"line 94"<<endl;
		if(strcmp(c.name, "aluminium"))
			comm = 0;
		else if(strcmp(c.name, "copper"))
			comm = 1;
		else if(strcmp(c.name, "cotton"))
			comm = 2;
		else if(strcmp(c.name, "crudeoil"))
			comm = 3;
		else if(strcmp(c.name, "gold"))
			comm = 4;
		else if(strcmp(c.name, "lead"))
			comm = 5;
		else if(strcmp(c.name, "menthaoil"))
			comm = 6;
		else if(strcmp(c.name, "naturalgas"))
			comm = 7;
		else if(strcmp(c.name, "nickel"))
			comm = 8;
		else if(strcmp(c.name, "silver"))
			comm = 9;
		else if(strcmp(c.name, "zinc"))
			comm = 10;
		if(commodities[comm].prevPrices.size()==4){
			commodities[comm].prevPrices.pop();
		}
		cout<<"line 98"<<endl;
		commodities[comm] = c;
		cout<<"line 100"<<endl;
		commodities[comm].prevPrices.push(c.currentPrice);
		queue<double> temp = commodities[comm].prevPrices;
		while (!temp.empty()) {
			cout << temp.front()<<" "<<endl;
			temp.pop();
		}
		cout<<"line 102	"<<endl;
		i=(i+1)%Size;
		//cout<<buffer->index<<endl;
		//buffer->index=(buffer->index+1)%size;
		cout<<commodities[comm].name<<" "<<commodities[comm].currentPrice<<endl;
		cout<<"line 107"<<endl;
		//al++;

	 
		//..
		sem_buf.sem_op=1;	//signal for s
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid0,&sem_buf,1);
		sem_buf.sem_op=1;	//signal for e
		sem_buf.sem_num=0;
		sem_buf.sem_flg=SEM_UNDO;
		r=semop(semid2,&sem_buf,1);

		// if(comm == 4){
		// 	cout<<"line 122"<<endl;
		// 	commodities.erase(commodities.begin());
		// 	comm --;
		// }
		// if(al == 3)
		// 	less4 = 0;
		cout<<"size"<<commodities[comm].prevPrices.size()<<endl;
		if(commodities[comm].prevPrices.size() == 4){
			cout<<"line 130"<<endl;
			queue<double> temp = commodities[comm].prevPrices;
			while (!temp.empty()) {
				commodities[comm].avgPrice += temp.front();
				//cout << temp.front()<<" "<<endl;
				temp.pop();
			}
		commodities[comm].avgPrice /= (comm + 1);
		cout<<"line 135"<<endl;
		}
		/*
		if(al != 0){
			if(commodities[al].price > commodities[al-1].price)
			{
				cout<<"before"<<endl;
				printf("%c",24);
				strcpy(arrow, " ↑ ");
				cout<<"after"<<endl;
			}
				
			// else if(commodities[al].price < commodities[al-1].price)
			// 	strcpy(arrow," ↓ ");
			// else
			// 	strcpy(arrow, " - ");
		}*/
		
		cout<<"success"<<endl;

		cout<<"+-------------------------------------+"<<endl;
		cout<<"| Currency | Price | AvgPrice |"<<endl;
		for(int k=0; k<11; k++){
			cout<<"+-------------------------------------+"<<endl;
			cout<<"| "<<commodities[k].name;
			printf("|%7.2lf %c ", commodities[k].currentPrice, 216);
			printf("|%7.2lf|\n",commodities[k].avgPrice);
			cout<<"+-------------------------------------+"<<endl;
		}
		
	  

		 
		
	}

}