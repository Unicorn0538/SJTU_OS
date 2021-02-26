#include "cpu.h"
void run(Task *task, int s) //程序运行
{
	printf("Running Task = [%s] [%d] [%d] [%d] for %d unites.\n",task->name,task->tid,task->priority,task->burst,s);
}

