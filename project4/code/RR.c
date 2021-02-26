#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "task.h"
#include "Schedule.h"

int N = 0;   
int select_count = 0;
int timeRun =0;

extern struct node **head;

Task *pickNextTask(int counter) //选择任务
{
	struct node *tmp;
	Task *task1;
	tmp = *head;
	while(tmp != NULL)
	{
		if(tmp->task->tid == select_count)
		{
			task1 = tmp->task;
		}
		tmp = tmp->next;
	}
	if(counter-1 > select_count)
		select_count++;
	else select_count=0;
	return task1;
}

void schedule(int counter)
{
	Task *tmp_task;
	struct node *tmp;
	tmp = *head;
	int burst[counter], turnaround[counter],waiting[counter],response[counter];
	
	for(int i=0; i<counter;i++)
	{
		burst[i]=0;
		turnaround[i]=0;
		waiting[i]=0;
		response[i]=0;
	}
	while(tmp !=NULL)
	{ burst[tmp->task->tid] = tmp->task->burst;
	  tmp = tmp->next;}
	
	
	while(1)
	{	
		int k=0;
		for(int i=0; i<counter;i++)
			k+=burst[i];
		if(k<=0)break;
		
		tmp_task = pickNextTask(counter);

		if(burst[tmp_task->tid]>0 && burst[tmp_task->tid]<=QUANTUM)
		{
			run(tmp_task,burst[tmp_task->tid]);
			if(response[tmp_task->tid]==0&&tmp_task->tid!=1)
				response[tmp_task->tid]=timeRun;
			timeRun += burst[tmp_task->tid];
			burst[tmp_task->tid]=0;
			turnaround[tmp_task->tid]=timeRun;
			waiting[tmp_task->tid]=turnaround[tmp_task->tid]-tmp_task->burst;
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

