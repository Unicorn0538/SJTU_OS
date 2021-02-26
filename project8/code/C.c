#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FRAME_SIZE 256  //physical memory
#define NUMBER_FRAME 128  

#define TLB_SIZE 16 //TLB

#define PAGE_TABLE_SIZE 128 

#define ADDRESS_MASK 0xFFFF
#define OFFSET_MASK 0xFF
 
typedef struct address_num
{
	int logical;
	int physical;
	signed char value;
	
}ADDRESS; 

ADDRESS address[2000];

typedef struct TLB_data
{
	int page;
	int frame;
	int valid;
}tlb;

tlb TLB[TLB_SIZE];

typedef struct Page_table_data
{
	int page;
	int frame;
	int valid;
}PAGE_TABLE;

PAGE_TABLE page_table[PAGE_TABLE_SIZE];

int physical_memory[NUMBER_FRAME][FRAME_SIZE];

int TLB_HIT = 0;
int PAGE_MISS =0;

int count_frame = 0;   //FIFO count
int count_page_table = 0;  //FIFO count
int count_TLB=0;    //FIFO count

FILE *fp;
FILE *backing;

void update(int page_number)
{
	//update physical
	count_frame=count_frame % NUMBER_FRAME;
	count_page_table %= PAGE_TABLE_SIZE; 
	
	signed char tmp[FRAME_SIZE];
	
	fseek(backing,page_number*FRAME_SIZE,SEEK_SET);
	fread(tmp,sizeof(signed char),FRAME_SIZE,backing);
	
	for(int i=0;i<FRAME_SIZE;i++)
		physical_memory[count_frame][i]=tmp[i];
	
	//update page table
	page_table[count_page_table].page = page_number;
	page_table[count_page_table].frame = count_frame;
	page_table[count_page_table].valid = 1;	

	count_page_table ++;
	count_frame ++;
	
}

void update_TLB(int page_number,int frame_number)
{
	count_TLB %= TLB_SIZE;
	TLB[count_TLB].page = page_number;
	TLB[count_TLB].frame = frame_number;
	TLB[count_TLB].valid = 1;
	count_TLB++;
}


void get_physical(int k)
{
	int page_number;
	int offset;
	int frame_number;

	//calculate page number and offset
	page_number= ((address[k].logical & ADDRESS_MASK)>>8);
	offset = (address[k].logical & OFFSET_MASK);
	
	
	int flag=0;
	
	//determine whether the TLB hit 
	for(int i=0;i<TLB_SIZE;i++)
	{
		if(TLB[i].valid==1&&TLB[i].page == page_number)
		{
			frame_number = TLB[i].frame;
			TLB_HIT ++;
			flag=1;
		}
		if(flag)break;

	}
	
	//determine whether the page table hit
	if(!flag)
	{
		for(int i=0; i<PAGE_TABLE_SIZE; i++)
		{
			if(page_table[i].valid==1&&page_table[i].page==page_number)
			{
				frame_number = page_table[i].frame;
				flag = 1;
				update_TLB(page_number,frame_number);
			}
			if(flag)break;
		}
	}
	
	//handing page faults
	if(!flag)
	{
		PAGE_MISS ++;
		update(page_number);
		frame_number = count_frame-1;
		update_TLB(page_number,frame_number);
	}
	
	address[k].value = physical_memory[frame_number][offset];
	address[k].physical = ((frame_number<<8)|offset);		
}

void display(int count)
{
	printf("address_logical		address_pythsical	value\n");
	for(int i=0; i<count; i++)
		printf("%d			%d			%d\n",address[i].logical,address[i].physical,address[i].value);

	printf("Page faults rate is : %.3f\n",(PAGE_MISS/(double)count));
	printf("TLB hits rate is : %.3f\n",(TLB_HIT/(double)count));

}

int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		printf("incorrect argument!\n");
		return 0;
	}
	
	backing = fopen("BACKING_STORE.bin","rb");
	if(backing==NULL)
	{
		printf("BACKING_STORE.bin open failed!\n");
		return 0;
	}
	
	fp = fopen(argv[1],"r");
	if(fp==NULL)
	{
		printf("adresses.txt open failed!\n");
		return 0;
	}


	for(int i=0;i<TLB_SIZE;i++)
		TLB[i].valid = 0;
	for(int i=0;i<PAGE_TABLE_SIZE;i++)
		page_table[i].valid = 0;	
	
	char num[10];
	int count=0;
	while(fgets(num,10,fp)!=NULL)
	{
		address[count].logical = atoi(num);		
		get_physical(count);
		count++;	
	}
	display(count);
	fclose(fp);
	fclose(backing);
	return 0;
}
