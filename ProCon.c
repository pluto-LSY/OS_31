//生产者消费者实现
#include<pthread.h>//包含线程库
#include<stdlib.h>
#include<stdio.h>
#include<semaphore.h>//信号量库
#define n 11;

//初始量定义
sem_t empty,full,mutex;
int buffer[10]={-1};
int fill=0;
int use=0;
//生产者放进去东西
void put(int value){
	buffer[fill]=value;
	fill=(fill+1)%n;
}
//消费者取东西
int get(){
	int tmp=buffer[use];
	use=(use+1)%n;
	return tmp;
}

//生产者进程
void *producer(){
	printf("producer\n");
	int i;
	for(i=0;i<=20;i++){
		//sleep(1);
		sem_wait(&empty);
		sem_wait(&mutex);//保证只有一个生产者进行临界区操作，防止数据的覆盖
		put(i);
		sem_post(&mutex);
		sem_post(&full);
	printf("p puts %d\n",i);
	}
}

//消费者过程
void *consumer(){
	printf("consumer\n");
	int i;
	for(i=0;i<=20;i++){
		//sleep(1);
		sem_wait(&full);
		sem_wait(&mutex);
		int tmp=get();
		sem_post(&mutex);
		sem_post(&empty);
		printf("c gets %d.\n",tmp);
	}
}

//创建线程
int main(){
	sem_init(&empty,0,10);
	sem_init(&full,0,0);
	sem_init(&mutex,0,1);

	pthread_t pro,con;
	pthread_create(&pro,NULL,producer,NULL);
	pthread_create(&con,NULL,consumer,NULL);
	
	pthread_join(con,NULL);
	pthread_join(pro,NULL);
}
	
