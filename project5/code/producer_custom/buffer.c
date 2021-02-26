#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
int head=0,tail=0,k=0;
int insert_item(buffer_item item)
{
	k=sem_wait(&empty);
	if(k==-1)return -1;
	pthread_mutex_lock(&mutex);
	buffer[tail]=item;
	tail = (tail+1)%BUFFER_SIZE;
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	return 0;
}

int remove_item(buffer_item *item)
{
	k=sem_wait(&full);
	if(k==-1)return -1;
	pthread_mutex_lock(&mutex);
	*item = buffer[head];
	head = (head+1)%BUFFER_SIZE;
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	return 0;
}


