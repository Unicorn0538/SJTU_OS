#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "task.h"
#include "Schedule.h"
#define priori 10
int N = 0;   
int select_count = 0;
int timeRun =0;

extern struct node **head;

Task *pickNextTask(int counter,int *pri[]) //选择任务
{
	int i;
	for(i=1;i<=priori;i++)
		if(pri[i][0]!=0)
			break;
	if(pri[i][select_count+1]!=0)
		select_count++;
	else select_count=0;
	struct node *tmp;
	Task *task1;
	tmp = *head;
	
	
	if(i>priori)return NULL;
	while(tmp != NULL)
	{
		if(tmp->task->tid+1 == pri[i][select_count])
		{
			task1 = tmp->task;
		}
		tmp = tmp->next;
	}
	
	return task1;
}

void schedule(int counter)
{
	Task *tmp_task;
	struct node *tmp;
	tmp = *head;
	int burst[counter], turnaround[counter],waiting[counter],response[counter],**pri;
        pri=(int **)malloc(sizeof(int*)*(priori+1));
	for(int i=0;i<priori+1;++i)
		pri[i]=(int*)malloc(sizeof(int)*(counter+2));	
	for(int i=0; i<counter;i++)
	{
		burst[i]=0;
		turnaround[i]=0;
		waiting[i]=0;
		response[i]=0;
		
	}
	for(int i=1;i<priori+1;i++)
		for(int j=0;j<counter+2;j++)
			pri[i][j]=0;

	while(tmp !=NULL)
	{ 	burst[tmp->task->tid] = tmp->task->burst;
	  	
       		int j=0;
		while(pri[tmp->task->priority][j]!=0)
			j++;
		pri[tmp->task->priority][j]=tmp->task->tid+1;
		tmp = tmp->next;	
	}

	
	while(1)
	{	
		int k=0;
		for(int i=0; i<counter;i++)
			k+=burst[i];
		if(k<=0)break;
		
		tmp_task = pickNextTask(counter,pri);

		if(burst[tmp_task->tid]>0 && burst[tmp_task->tid]<=QUANTUM)
		{
			run(tmp_task,burst[tmp_task->tid]);
			if(response[tmp_task->tid]==0&&tmp_task->tid!=1)
				response[tmp_task->tid]=timeRun;
			timeRun += burst[tmp_task->tid];
			burst[tmp_task->tid]=0;
			turnaround[tmp_task->tid]=timeRun;
			waiting[tmp_task->tid]=turnaround[tmp_task->tid]-tmp_task->burst;

			 k=0;			
			while(pri[tmp_task->priority][k]!=tmp_task->tid+1&&k<counter)k++;
			pri[tmp_task->priority][k++]=0;
			while(pri[tmp_task->priority][k]!=0&&k<counter)
			{pri[tmp_task->priority][k-1]=pri[tmp_task->priority][k];k++;}
			pri[tmp_task->priority][k-1]=0;
			pri[tmp_task->priority][k]=0;
			select_count--;
		}
		else if(burst[tmp_task->tid]>QUANTUM)
		{
			run(tmp_task,QUANTUM);
			if(response[tmp_task->tid]==0)
				response[tmp_task->tid] = timeRun;
			timeRun += QUANTUM;
			burst[tmp_task->tid]-=QUANTUM;
		}
			
	}


	compute_time(counter,turnaround, waiting, response ); 

}

