#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMBER_OF_CUSTOMERS 10
#define NUMBER_OF_RESOURCES 5

int avaliable[NUMBER_OF_RESOURCES];
int maxium[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int count1=0; //number of customers
int count2=0; //number of resoucers


int is_safe()  //safe=1; unsafe=0 
{
	int work[count2],finish[count1],flag=1;
	for(int i=0;i<count2;i++)
		work[i] = avaliable[i];
	for(int i=0;i<count1;i++)
		finish[i]=0;
	
	for(int i=0;i<count1;i++)//judge finih[i]==false && need_i <= work
	{
		if(finish[i] == 0)
		{
			flag=1;
			for(int j=0; j<count2; j++)
				if(need[i][j]>work[j])
					flag =0;	
			if(flag)
			{
				finish[i]=1;
				for(int k=0;k<count2;k++)
					work[k] += allocation[i][k];
				i=-1;
			}
		}
	}
	
	for(int i=0;i<count1;i++)
		if(!finish[i])
			return 0;
	return 1;
	
}

int request_resources(int customer,int request[])
{
	if(customer<0 || customer >= count1)
		return 0;
	for(int i=0;i<count2;i++) //judge Request<=Need && Request<=Avaliable
	{	if(request[i]<0 )
			return 0;
		if(request[i]>avaliable[i] || request[i] > need[customer][i])
			return -1;
	}	
	for(int i=0;i<count2;i++) //fulfill request
	{
		avaliable[i]-=request[i];
		allocation[customer][i]+=request[i];
		need[customer][i]-=request[i];
	}
	if(!is_safe()) //determine whether to recover
	{
		for(int i=0;i<count2;i++)
		{
			avaliable[i]+=request[i];
			allocation[customer][i]-=request[i];
			need[customer][i]+=request[i];
		}
		return -2;
	}
	return 1;
}

int release_resources(int customer,int release[])
{
	if(customer<0 || customer >= count1)
		return -1;
	for(int i=0;i<count2;i++) 
		if(release[i]<0 )
			return -1;

	for(int i=0; i<count2; i++)
	{
		allocation[customer][i]-=release[i];
		if(allocation[customer][i]<0)
		{
			allocation[customer][i]+=release[i];			
			for(int j=0;j<i;j++)
			{
				avaliable[j]-=release[j];
				allocation[customer][j]+=release[j];
				need[customer][j]-=release[j];
			}
			return 1;
		}
		
		need[customer][i]+=release[i];				
		avaliable[i]+=release[i];	
	}
	return 0;
}

void display()
{
	printf("============================================\n");
	printf("Avaliable:\n");
	for(int i=0; i<count2;i++)
		printf("%d  ",avaliable[i]);
	printf("\n");
	printf("Maxium Matrix:\n");
	for(int i=0; i<count1;i++)
	{	
		for(int j=0; j<count2; j++)
			printf("%d  ",maxium[i][j]);
		printf("\n");
	}
	printf("Allocation Matrix:\n");
	for(int i=0; i<count1;i++)
	{	
		for(int j=0; j<count2; j++)
			printf("%d  ",allocation[i][j]);
		printf("\n");
	}
	printf("Need Matrix:\n");
	for(int i=0; i<count1;i++)
	{	
		for(int j=0; j<count2; j++)
			printf("%d  ",need[i][j]);
		printf("\n");
	}
	printf("============================================\n");
}

int main(int argc,char *argv[])
{
	//initialized 
	if(argc > 2+NUMBER_OF_RESOURCES)
	{
		printf("INCORRECT NUMEBR OF PARAMETER!\n");
		return -1;
	}
	count2 = argc-2;
	for(int i=0;i<count2;i++)
		avaliable[i] = atoi(argv[i+1]);
	FILE *fp = fopen(argv[count2+1],"r");
	if(!fp)
	{
		printf("OPEN FAILED!\n");
		return -1;
	}
	char *tmpchar,task[100];
	while(fgets(task,100,fp)!=NULL)   
	{
		tmpchar = strdup(task);
		for(int i=0;i<count2;i++)
			maxium[count1][i] = atoi(strsep(&tmpchar,","));
		count1++;
		free(tmpchar);
		if(count1>NUMBER_OF_CUSTOMERS)
		{
			printf("INCORRECT NUMEBR OF CUSTOMERS!\n");
			return -1;
		}
	}
	fclose(fp);
	
	for(int i=0;i<count1;i++)
		for(int j=0;j<count2;j++)
		{
			allocation[i][j] = 0;
			need[i][j] = maxium[i][j];
		}
	printf("Initial successed!\n");
	printf(">>>");
	

	char str[5];
	int tmp[count2+1],cus_tmp,flag=0;
	while(scanf("%s",str)==1)
	{
		if(strcmp(str,"RQ")==0)
		{
			scanf("%d",&cus_tmp);
			for(int i=0;i<count2;i++)
				scanf("%d",&tmp[i]);
			flag=request_resources(cus_tmp,tmp);
			if(flag==1)
				printf("The request is safe,accepted!\n");
			else if(flag==-2)
				printf("The request is unsafe,denied!\n");
			else if(flag==-1)
				printf("Insufficient resources, request denied!\n");
			else if(flag==0)
				printf("Input Error!\n");
		}
		else if(strcmp(str,"RL")==0)
		{
			scanf("%d",&cus_tmp);
			for(int i=0;i<count2;i++)
				scanf("%d",&tmp[i]);
			flag=release_resources(cus_tmp,tmp);
			if(flag==0)
				printf("The release request is accepted!\n");
			else if(flag==1)
				printf("The release request is denied!\n");
			else if(flag==-1)
				printf("Input Error!\n");
		}
		else if(strcmp(str,"*")==0)
		{
			display();
		}
		else if(strcmp(str,"quit")==0)
			break;
		else
			printf("INPUT ERROR!\n");
		printf(">>>");

	}
	return 0;
}
