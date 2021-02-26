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
	while(tmp != NULL)
	{
		if(tmp->task->tid == select_count)
		{
			task1 = tmp->task;
			select_count++;
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
	
	for(int i=0; i<counter;i++)  //核心算法
	{
		tmp_task = pickNextTask();
		run(tmp_task,tmp_task->burst);
		timeRun+=tmp_task->burst;
		if(tmp_task->tid == 1)
		{
			waiting[i]=0;
			response[i]=0;
		}
		else
		{
			waiting[i] = timeRun - tmp_task->burst;
			response[i] = timeRun - tmp_task ->burst;
		}
		turnaround[i]=timeRun;
	}
	
	compute_time(counter,turnaround, waiting, response ); 

}

	


