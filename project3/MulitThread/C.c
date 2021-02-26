#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


int count;
int sort_array[100];

void *Sort_First(void *param) //冒泡排序前半部分
{
		
	int *array=(int *)param;
	int tmp;
	int flag=0;
	for(int i=0;i<count/2;i++)
	{	
		for(int j=0;j<count/2-1-i;j++)
			if(array[j]>array[j+1])
			{
				tmp=array[j];
				array[j]=array[j+1];
				array[j+1]=tmp;
				flag=1;			
			}
		if (!flag) break;
	}
	pthread_exit(0);			
}

void *Sort_Second(void *param) //冒泡排序后半部分
{
	int *array=(int *)param;
	int tmp;
	int flag=0;
	for(int i=0;i<count/2+1;i++)
	{	
		for(int j=count/2;j<count-i-1;j++)
			if(array[j]>array[j+1])
			{
				tmp=array[j];
				array[j]=array[j+1];
				array[j+1]=tmp;
				flag=1;			
			}
		if (!flag) break;
	}
	pthread_exit(0);			
}

void Merge_Array(int *array)
{
	int i=0,j=count/2,k=0;
	while(i<count/2&&j<count)
	{
		if(array[i]<array[j])
			sort_array[k++]=array[i++];
		if(array[i]>=array[j])
			sort_array[k++]=array[j++];
	}
	while(i<count)
		sort_array[k++]=array[i++];
	while(j<count)
		sort_array[k++]=array[j++];
}

int main(int argc,char **argv)
{
	count=argc-1; //得到输入的长度
	int array[count];

	pthread_t tid1,tid2;   //定义子线程
	pthread_attr_t attr1,attr2;
	
	for(int i=0;i<argc-1;i++)    //得到数组元素
		array[i]=atoi(argv[i+1]);
	
	printf("the init array is :");
	for(int i=0; i<count; i++)
		printf("%d\t",array[i]);
	printf("\n");
	
	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);
	pthread_create(&tid1,&attr1,&Sort_First,array);  //子线程进行排序
	pthread_join(tid1,NULL);	
	
	pthread_create(&tid2,&attr2,&Sort_Second,array);  //子线程进行排序
	pthread_join(tid2,NULL);	

	printf("before merge,the array is :");
	for(int i=0; i<count; i++)
		printf("%d\t",array[i]);
	printf("\n");
	
	Merge_Array(array);  //合并
	printf("After sort,the array is :");
	for(int i=0; i<count; i++)
		printf("%d\t",sort_array[i]);
	printf("\n");
	return 0;
}
