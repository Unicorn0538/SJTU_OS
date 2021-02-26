#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
#define SLEEP_TIME 6
#define true 1
void *producer(void *param)
{
	buffer_item item;
	
	while(true)
	{
		sleep(rand()%SLEEP_TIME+1);
		item = rand();
		if(insert_item(item))
			printf("report error condition!");
		else 
			printf("producer produced %d\n",item);
	}
	
}

void *consumer(void *param)
{
	buffer_item item;
		
	while(true)
	{
		sleep(rand()%SLEEP_TIME+1);
		if(remove_item(&item))
			printf("report error condition");
		else
			printf("consumer consumed %d\n",item);
	}
}

int main(int argc, char *argv[])
{
	
	if(argc<4)
	{
		fprintf(stderr,"need at least 3 args\n");
		return -1;
	}
	if(atoi(argv[1])<=0 || atoi(argv[2])<=0 || atoi(argv[3])<=0)
	{
		fprintf(stderr,"Input Error!\n");
		return -1;
	}
        //initialize
	pthread_mutex_init(&mutex,NULL);
	sem_init(&full,0,0);
	sem_init(&empty,0,BUFFER_SIZE);
	
	//create producer threads
	for(int i=0; i<atoi(argv[2]);++i)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid,&attr,producer,NULL);
	}
	
	//create consumer threads
	for(int i=0; i<atoi(argv[3]);++i)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid,&attr,consumer,NULL);
	}

	//sleep
	sleep(atoi(argv[1]));

	//exit
	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_mutex_destroy(&mutex);
	exit(0);
}
