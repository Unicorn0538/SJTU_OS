#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "Schedule.h"

struct node **head;
int task_count=0;  //Task的计数
int main(int argc,char *argv[])
{
	char *name,task[100];
	
	int priority=0,burst=0,value=0;
	struct node **tmpnode = malloc(sizeof(struct node));
	head = tmpnode;
	FILE *in = fopen(argv[1],"r"); //打开文件
	if(in == NULL)
	{
		printf("Open failed!\n");
		return -1;
	}
	
	char *tmpchar;
	while(fgets(task,100,in)!=NULL)   //将所有任务加载到head后
	{
		tmpchar = strdup(task);
		name = strsep(&tmpchar,",");
		priority = atoi(strsep(&tmpchar,","));
		burst = atoi(strsep(&tmpchar,","));
		add(name,priority,burst,value);
		task_count++;
		value ++;
		//__sync_fetch_and_add(&value,1);
		free(tmpchar);
	}
	
	fclose(in);
	schedule(task_count); //调用调度算法
	
	return 0;
}
