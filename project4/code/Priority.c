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

Task *pickNextTask() //选择任务
{
	struct node *tmp;
	Task *task1;
	tmp = *head;
	int st = tmp->task->priority;
	while(tmp != NULL)
	{
		if(tmp->task->priority <= st)
		{
			st = tmp->task->priority;
			task1 = tmp->task;
		}
		tmp = tmp->next;
	}
	return task1;
}

void schedule(int counter)
{
	Task *tmp_task;
	int turnaround[counter],waiting[counter],response[counter];
	
	for(int i=0; i<counter;i++)
	{
		turnaround[i]=0;
		waiting[i]=0;
		response[i]=0;
	}
	
	for(int i =0 ;i<counter;i++)
	{
		tmp_task = pickNextTask();
		run(tmp_task,tmp_task->burst);
		timeRun += tmp_task->burst;
		if(i==0)
		{
			waiting[i]=0;
			response[i]=0;
		}
		else
		{
			waiting[i] = timeRun - tmp_task->burst;
			response[i] = timeRun - tmp_task->burst;
		}
		turnaround[i] = timeRun;
		delete(head,tmp_task);		
	}


	compute_time(counter,turnaround, waiting, response ); 

}

