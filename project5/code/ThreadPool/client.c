#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "threadpool.h"

struct data
{
	int a;
	int b;
};


void add(void *param)
{
	struct data *tmp;
	tmp = (struct data*)param;
	printf("I add two values %d and %d result = %d\n",tmp->a,tmp->b,tmp->a+tmp->b);
}

int main(void)
{
	int n=0;
	while(n<=0)
	{
		scanf("%d",&n);
	}
	
	pool_init();
	
	struct data work[n];
	for(int i=0;i<n;i++)
	{
		work[i].a = i;
		work[i].b = -i;
		while(pool_submit(&add,&work[i])!=0)
		{sleep(1);printf("\nwaiting!\n\n");sleep(2);}
	}
        	
	sleep(3);
	
	pool_shutdown();

	return 0;
}
			
