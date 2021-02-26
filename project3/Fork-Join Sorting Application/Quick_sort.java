import java.util.concurrent.*;
import java.util.Scanner;

public class Quick_sort {

    public static final int MaxSize = 10;
   // public static final int Array_size = 20;
    
    public static void main(String [] args)
    {
	Scanner s = new Scanner(System.in);
	String inputstr = s.nextLine();
	String [] str = inputstr.split(" ");
	int [] Array = new int [str.length];
	for(int i=0; i<Array.length;i++)
		Array[i] = Integer.parseInt(str[i]);
	ForkJoinPool pool = new ForkJoinPool();
	
	if (Array.length < MaxSize)
	{	Select_sort(Array,Array.length);
		System.out.print("Select_sort: ");
	}
	else
	{
		Sort_quick quick = new Sort_quick(0,Array.length-1,Array);
		pool.invoke(quick);
		System.out.print("QuickSort: ");	
	}
	for(int i=0; i<Array.length;i++)
		System.out.printf("%d\t",Array[i]);
	System.out.print("\n");
    }
    
    public static void Select_sort(int [] array,int size)
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

class Sort_quick extends RecursiveAction
{
	
	
	private int left;
	private int right;
	private int [] arr;
	
	public Sort_quick(int left,int right,int [] arr)
	{
		this.left  = left;
		this.right = right;
		this.arr = arr;
	}
	
	protected void compute()
	{
		if (left >= right)
			return ;
		int tmp = part(left,right,arr);
		RecursiveAction leftSort  = new Sort_quick(left,tmp-1,arr);
		RecursiveAction rightSort  = new Sort_quick(tmp+1,right,arr);
		invokeAll(leftSort,rightSort);
	}
	
	public static int part(int left,int right,int[]a)
	{
		int tmp = a[left];
		int l = left,r=right;
		while(l<r)
		{	
			while(l<r&&a[r]>=tmp)
				r--;
			a[l]=a[r];
			while(l<r&&a[l]<=tmp)
				l++;
			a[r]=a[l];
		}
		a[l]=tmp;
		return l;
	}
}

