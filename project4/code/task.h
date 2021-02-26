#ifndef TASK_H
#define TASK_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct task
{
	char *name;
	int   priority;
	int   tid;
	int   burst;
}Task;

struct node
{
	Task *task;
	struct node *next;
};

void schedule(int counter );
#endif
