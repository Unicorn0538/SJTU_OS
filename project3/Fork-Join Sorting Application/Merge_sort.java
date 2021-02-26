import java.util.concurrent.*;
import java.util.Scanner;

public class Merge_sort {

    public static final int MaxSize = 10; //数组规模大于MaxSize，选择MergeSort
  
    public static void main(String [] args)
    {
	Scanner s = new Scanner(System.in);  //将键盘输入的由空格划分的字符串变为数组
	String inputstr = s.nextLine();
	String [] str = inputstr.split(" ");
	int [] Array = new int [str.length];
	for(int i=0; i<Array.length;i++)
		Array[i] = Integer.parseInt(str[i]);
	ForkJoinPool pool = new ForkJoinPool();  //定义线程池
	
	if (Array.length < MaxSize)         //规模小时用选择排序
	{	Select_sort(Array,Array.length);
		System.out.print("Select_sort: ");
	}
	else    
	{
		Sort_merge mer = new Sort_merge(0,Array.length-1,Array);
		pool.invoke(mer);
		System.out.print("MergeSort: ");	
	}
	for(int i=0; i<Array.length;i++)
		System.out.printf("%d\t",Array[i]);
	System.out.print("\n");
    }
    
    public static void Select_sort(int [] array,int size) //选择排序
    {
	int min=50000,flag=0,tmp;
	for(int i=0;i<size;i++)
	{
		for(int j=i;j<size;j++)
		{
			if (array[j]<min)
				{min=array[j];flag=j;}
		}
		array[flag]=array[i];
		array[i]=min;
		min=50000;
		flag=i+1;
	}	
    }   
}

class Sort_merge extends RecursiveAction //扩展类
{
	
	
	private int left;
	private int right;
	private int [] arr;
	
	public Sort_merge(int left,int right,int [] arr)
	{
		this.left  = left;
		this.right = right;
		this.arr = arr;
	}
	
	protected void compute()
	{
		if (left >= right)
			return ;
		int mid = (left + right)/2;
		RecursiveAction leftSort  = new Sort_merge(left,mid,arr);  
		RecursiveAction rightSort  = new Sort_merge(mid+1,right,arr);
		invokeAll(leftSort,rightSort);  //线程执行
		merge(left,mid,right,arr);

	}
	
	public static void merge(int left,int mid,int right,int [] array) //MergeSort
	{
		int tmp[] = new int [right-left+1];
		int i = left;
		int j = mid +1;
		int k = 0;
		while(i<=mid && j<=right)
		{
			if(array[i]<=array[j])
				tmp[k++]=array[i++];
			else
				tmp[k++]=array[j++];
		}
		while(i<=mid)
			tmp[k++]=array[i++];
		while(j<=right)
			tmp[k++]=array[j++];
	
		for (int x = 0; x < tmp.length; x++)
			array[left+x]=tmp[x];
	}
}

