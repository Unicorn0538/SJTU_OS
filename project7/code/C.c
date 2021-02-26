#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
typedef struct Mem_block
{
	int start;
	int end;
	char name[10];
}BLOCK;
	
BLOCK block[1000];
int block_len=0;
int size=0;

void insert(char name[],int N,int k)
{	
	
	int tmp=0;
	for(int i=size;i>k;i--)
	{
		block[i].start=block[i-1].start;
		block[i].end = block[i-1].end;
		strcpy(block[i].name,block[i-1].name);
	}
	if(k==0)
		block[k].start=0;	
	else	
		block[k].start=block[k-1].end+1;
	block[k].end=block[k].start+N-1;
	strcpy(block[k].name,name);
	size++;
}

int request(char name[],int N, char type)
{
	if(N<=0)
	{
		printf("size of block is error!\n");
		return -1;
	}	
	int block_tmp[size+1]; //sizes between two blocks
	if(size==0)
		block_tmp[0]=block_len;	
	else block_tmp[0]=block[0].start;  
	for(int i=1;i<size;i++)
		block_tmp[i]=block[i].start-block[i-1].end-1;
	block_tmp[size]=block_len-block[size-1].end;
	
	int j;
	if(type=='W')
	{
		int tmp=0,flag=0;
		for(j=0;j<size+1;j++)
			if(tmp<block_tmp[j])
			{	tmp=block_tmp[j];
				flag=j;		}
		if(tmp<N)
		{
			printf("No enough space!\n");
			return -1;
		}
		insert(name,N,flag);

	}
	else if(type=='B')
	{
		int best=10000000,flag=0,k=0;
		for(j=0;j<size+1;j++)
		{
			if(block_tmp[j]>=N && block_tmp[j] <=best)
			{
				best=block_tmp[j];
				flag=1;
				k=j;
			}
		}
		if(!flag)
		{
			printf("No enough space!\n");
			return -1;
		}
		insert(name,N,k);
		
	}
	else if(type=='F')
	{
		for(j=0;j<size+1;j++)
			if(block_tmp[j]>=N)
				break;
		if(j==size+1)	
		{
			printf("No enough space!\n");
			return -1;
		}	
		insert(name,N,j);
		
	}
	else
	{
		printf("No such strategy!\n");
		return 1;
	}
	
	return 0;
}

void release(char name[])
{
	int i;
	for(i=0;i<size;i++)
		if(strcmp(name,block[i].name)==0)
			break;
	for(int j=i;j<size-1;j++)
	{
		block[j].start = block[j+1].start;
		block[j].end = block[j+1].end;
		strcpy(block[j].name,block[j+1].name);
	}
	size--;
	printf("The release request is accepted!\n");
}

void compat()
{
	if(size<=0){printf("memory is unused\n");return;}
	int tmp=0;
	if(block[0].start!=0)
	{
		tmp = block[0].end - block[0].start;
		block[0].start=0;
		block[0].end=0+tmp;
	}
	for(int i=1;i<size;i++)
	{
		if(block[i].start-1!=block[i-1].end)
		{
			tmp = block[i].end - block[i].start;
			block[i].start=block[i-1].end+1;
			block[i].end=block[i].start+tmp;
		}
	}
	printf("cpmpat success!\n");
}

void display()
{
	printf("============================================\n");
	if(size==0)
	{
		printf("Addresses  [ 0 : %d ] unused \n",block_len);
		printf("============================================\n");
		return;
	}
	if(block[0].start==0)
		printf("Addresses  [ %d : %d ] process %s\n",block[0].start,block[0].end,block[0].name);
	else
	{	printf("Addresses  [ 0 : %d ] unused \n",block[0].start-1);
		printf("Addresses  [ %d : %d ] process %s\n",block[0].start,block[0].end,block[0].name);	}
	for(int i=1; i<size; i++)
	{
		
		if(block[i-1].end+1 == block[i].start)
			printf("Addresses  [ %d : %d ] process %s\n",block[i].start,block[i].end,block[i].name);
		else 
			{	printf("Addresses  [ %d : %d ] unused \n",block[i-1].end+1,block[i].start-1);
				printf("Addresses  [ %d : %d ] process %s\n",block[i].start,block[i].end,block[i].name);	}
	}
	if(block[size-1].end < block_len)
		printf("Addresses  [ %d : %d ] unused \n",block[size-1].end+1,block_len);
	printf("============================================\n");
}
int main(int argc,char *argv[])
{
	if(argc != 2)
		return 0;
	
	block_len = atoi(argv[1]); 	
	char str[10],name[10],allocate_type;
	int allocate_size;
	printf("allocator>>");
	while(scanf("%s",str)==1)
	{
		if(strcmp(str,"RQ")==0)
		{
			scanf("%s %d %c",name,&allocate_size,&allocate_type);
			if(request(name,allocate_size,allocate_type)==0)
				printf("Request is accepted!\n");
		}
		else if(strcmp(str,"RL")==0)
		{
			scanf("%s",name);
			release(name);
				
		}
		else if(strcmp(str,"C")==0)
		{
			compat();
		}
		else if(strcmp(str,"STAT")==0)
		{
			display();
		}
		else if(strcmp(str,"X")==0)
			break;
		else
			printf("Input Error!\n");		
		printf("allocator>>");
	}
	return 0;
}
