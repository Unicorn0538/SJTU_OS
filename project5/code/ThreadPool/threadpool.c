#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3
#define TRUE 1



typedef struct 
{
	void (*function)(void *p);
	void *data;
}task;


task worktodo,worksub,task_q[QUEUE_SIZE+1];
size_t head=0,tail=0;
sem_t semlock;


pthread_t bee[NUMBER_OF_THREADS];
pthread_mutex_t mutex;

int equeue(task t)
{
	pthread_mutex_lock(&mutex);
	if((tail+1)%(QUEUE_SIZE+1)==head)
	{pthread_mutex_unlock(&mutex);return 1;}

	task_q[tail].function = t.function;
	task_q[tail].data = t.data;
	tail=(tail+1)%(QUEUE_SIZE+1);
	pthread_mutex_unlock(&mutex);
	return 0;
}

task dequeue()
{
	task tmp;
	pthread_mutex_lock(&mutex);
	tmp.function=task_q[head].function;
	tmp.data=task_q[head].data;
	head=(head+1)%(QUEUE_SIZE+1);
	pthread_mutex_unlock(&mutex);
	
	return tmp;
}

void *worker(void *param)
{
	while(TRUE)
	{
		sem_wait(&semlock);
		//pthread_mutex_lock(&mutex);
		task tmp = dequeue();
		execute(tmp.function,tmp.data);
		//pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}

void execute(void(*somefunction)(void *p),void *p)
{
	(*somefunction)(p);

}

int pool_submit(void (*somefunction)(void *p),void *p)
{
	worksub.function = somefunction;
	worksub.data = p;
	size_t flag =0;
	flag = equeue(worksub);
	if(!flag)
		sem_post(&semlock);
	return flag;
}

void pool_init(void)
{
	for(int i=0;i<NUMBER_OF_THREADS;i++)	
		pthread_create(&bee[i],NULL,worker,NULL);
	if(pthread_mutex_init(&mutex,NULL)!=0 || sem_init(&semlock,0,0)!=0)
		{printf("initial wrong!\n");return ;}
	printf("Initial success!\n");
	
}

void pool_shutdown(void)
{
	for(int i=0;i!=NUMBER_OF_THREADS;i++)
        {		pthread_cancel(bee[i]);
		pthread_join(bee[i],NULL);
	}
	sem_destroy(&semlock);
	pthread_mutex_destroy(&mutex);
	printf("shutdown success!\n");
}

