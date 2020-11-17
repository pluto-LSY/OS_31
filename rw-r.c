//读者优先
#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/types.h>
#include<time.h>
#include<unistd.h>
#include<string.h>

//信号量设置
sem_t rmutex,wmutex;
int readcount;

struct data{
	int id;
	int opTime;
	int lastTime;
};

//reader
void* Reader(void *param){
	int id =((struct data*)param)->id;
	int lastTime = ((struct data*)param)->lastTime;
	int opTime=((struct data*)param)->opTime;
	
	sleep(opTime);//the op time
	printf("Thread %d : waiting to read\n",id);
	sem_wait(&rmutex);
	if(readcount==0){
		sem_wait(&wmutex);
	}
	readcount++;
	sem_post(&rmutex);
	printf("Thread %d: start reading\n",id);
	sleep(lastTime);
	printf("Thread %d: ending reading\n",id);
	
	sem_wait(&rmutex);
	readcount--;
	if(readcount==0) sem_post(&wmutex);	
	sem_post(&rmutex);
}

//writer
void* Writer(void* param){
	int id =((struct data*)param)->id;
	int lastTime = ((struct data*)param)->lastTime;
	int opTime=((struct data*)param)->opTime;
	
	sleep(opTime);
	printf("Thread %d:waiting to write\n",id);
	sem_wait(&wmutex);

	printf("Thread %d: start writing\n",id);
	sleep(lastTime);
	printf("Thread %d: ending writing\n",id);

	sem_post(&rmutex);
}

void main(){
	pthread_t tid;
	pthread_attr_t attr;//需要对结构体初始化
	
	pthread_attr_init(&attr);

	sem_init(&rmutex,0,1);
	sem_init(&wmutex,0,1);
	readcount=0;

	int id=0;
	while(scanf("%d",&id)!=EOF){
		char role;
		int opTime;
		int lastTime;

		scanf("%c%d%d",&role,&opTime,&lastTime);
		struct data* d = (struct data*)malloc(sizeof(struct data));

		d->id=id;
		d->opTime=opTime;
		d->lastTime=lastTime;

		if(role=='R'){
			printf("Create the %d thread:Reader\n",id);
			pthread_create(&tid,&attr,Reader,d);
			//pthread_join(tid,NULL);
		}
		if(role=='W'){
			printf("Create the %d thread:Writer\n",id);
			pthread_create(&tid,&attr,Writer,d);
			//pthread_join(tid,NULL);
		}
	}
	//sem_destroy(&rmutex);
	//sem_destory(&wmutex);

}

