#include "Schedule.h"
extern struct node **head;
void insert(struct node **tmphead,Task *t)
{
	struct node *N = malloc(sizeof(struct node));
	N->task = t;
	N->next = *tmphead;
	*tmphead = N;
}

void delete(struct node **tmphead,Task *t)
{
	struct node *tmp1,*tmp2;
	tmp1 = *tmphead;
	if(strcmp(t->name,tmp1->task->name)==0)
		*tmphead = (*tmphead)->next;
	else 
	{
		tmp2 = *tmphead;
		tmp1 = tmp1->next;
		while(strcmp(t->name,tmp1->task->name)!=0)
		{
			tmp2 = tmp1;
			tmp1 = tmp1->next;
		}
		tmp2->next = tmp1->next;
	}
}


void add(char *name,int priority,int burst,int N)
{
	//count ++ ;
	Task *t = malloc(sizeof(Task));
	t->name = name;
	t->priority = priority;
	t->burst = burst;
	t->tid = N;
	insert(head,t);	
}

void compute_time(int count,int *turnaround, int *waiting, int *response ) //计算平均时间
{
	int t=0,w=0,r=0;
	for(int i=0; i<count; i++)
	{
		t += turnaround[i];
		w += waiting[i];
		r += response[i];
	}
	printf("Average_Turnaround_Time: %f \n",t*1.0/count);
	printf("Average_Waiting_Time: %f \n",w*1.0/count);
	printf("Average_Reponse_Time: %f \n",r*1.0/count);
}

