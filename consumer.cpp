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
using namespace std; 
struct consumer{
	char name[10];//1 byte
	double price;//8 byte
	int index;//4yte	
	double avgPrice = 0; //8 byte 
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
#define size 5
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
	sem_val.val=size;
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
	int shmid =shmget(key3,size,0666 | IPC_CREAT);
	consumer *buffer=(consumer *)shmat(shmid,0,0);
	vector<struct consumer> alumin;
	consumer c;
	int al = 0;
	int less4 = 1;
	char arrow;
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
		strcpy(c.name,buffer[i].name);
		c.price=buffer[i].price;
		alumin.push_back(c);
		i=(i+1)%size;
		//cout<<buffer->index<<endl;
		//buffer->index=(buffer->index+1)%size;
		cout<<alumin[al].name<<" "<<alumin[al].price<<endl;
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

		if(strcmp(c.name, "aluminium") == 0){
      	cout<<"line 116"<<endl;
		if(al == 4){
			cout<<"line 118"<<endl;
			alumin.erase(alumin.begin());
			al --;
		}
		if(al == 3)
			less4 = 0;
		if(!less4){
			for(int i=0; i<=al; i++){
			cout<<"line 124"<<endl;
			alumin[al].avgPrice += alumin[i].price;
			cout<<"line 126"<<endl;
			}
		alumin[al].avgPrice /= (al + 1);
		}
		// if(al != 0){
		// 	if(alumin[al].price > alumin[al-1].price)
		// 		arrow = ''
		// }
		
		cout<<"success"<<endl;

		cout<<"+-------------------------------------+"<<endl;
		cout<<"| Currency | Price | AvgPrice |"<<endl;
		cout<<"+-------------------------------------+"<<endl;
		cout<<"| "<<alumin[al].name <<"| "<<alumin[al].price <<"| "<<alumin[al].avgPrice<<" |"<<endl;
		al++;
	  }

		 
		
	}

}