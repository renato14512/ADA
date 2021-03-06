#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define SIZE          8192  /* buffer size for reading /proc/<pid>/status */

using namespace std;

int mem_total ()
{
  char a[SIZE], *p, *q;
  int data, stack;
  int n, v, fd;
  pid_t pid= getpid();

  p = a;
  sprintf (p, "/proc/%d/status", pid);
  fd = open (p, O_RDONLY);
  if (fd < 0)
    return -1;
  do
    n = read (fd, p, SIZE);
  while ((n < 0) && (errno == EINTR));
  if (n < 0)
    return -2;
  do
    v = close (fd);
  while ((v < 0) && (errno == EINTR));
  if (v < 0)
    return -3;
  data = stack = 0;
  q = strstr (p, "VmData:");
  if (q != NULL)
    {
      sscanf (q, "%*s %d", &data);
      q = strstr (q, "VmStk:");
      if (q != NULL)
    sscanf (q, "%*s %d\n", &stack);
    }
  return (data + stack);
}


int mem_total (int &data, int &stack)
{
  char a[SIZE], *p, *q;
  int n, v, fd;
  pid_t pid= getpid();

  p = a;
  sprintf (p, "/proc/%d/status", pid);
  fd = open (p, O_RDONLY);
  if (fd < 0)
    return -1;
  do
    n = read (fd, p, SIZE);
  while ((n < 0) && (errno == EINTR));
  if (n < 0)
    return -2;
  do
    v = close (fd);
  while ((v < 0) && (errno == EINTR));
  if (v < 0)
    return -3;
  data = stack = 0;
  q = strstr (p, "VmData:");
  if (q != NULL)
    {
      sscanf (q, "%*s %d", &data);
      q = strstr (q, "VmStk:");
      if (q != NULL)
    sscanf (q, "%*s %d\n", &stack);
    }
  return (data + stack);
}
void CopyArray(int A[],int  iBegin,int iEnd,int B[])
{
    for(int k = iBegin; k < iEnd; k++)
        B[k] = A[k];
}
void TopDownMerge(int A[],int  iBegin,int iMiddle,int iEnd,int B[])
{
    int i = iBegin, j = iMiddle;
 
    
    for (int k = iBegin; k < iEnd; k++) {
        
        if (i < iMiddle && (j >= iEnd || A[i] <= A[j])) {
            B[k] = A[i];
            i = i + 1;
        } else {
            B[k] = A[j];
            j = j + 1;
        }
    }
}
void TopDownSplitMerge(int B[],int iBegin,int iEnd,int A[])
{
    if(iEnd - iBegin < 2)                       
        return;                                 
    
    int iMiddle = (iEnd + iBegin) / 2;              
    
    TopDownSplitMerge(A, iBegin,  iMiddle, B);  
    TopDownSplitMerge(A, iMiddle,    iEnd, B);  
    TopDownMerge(B, iBegin, iMiddle, iEnd, A);
}

void TopDownMergeSort(int A[],int B[],int n)
{
    CopyArray(A, 0, n, B);           
    TopDownSplitMerge(B, 0, n, A);   
}


void inSort(int A[], int n)  
{  
    int key,j;
    for (int i = 1; i < n; i++) 
    {  
        key=A[i];  
        j=i-1;
        while(j>=0 && *(A+j)>=key) 
        {
			A[j+1]=A[j];
            j=j-1;

        }  
        A[j+1] = key;
		
    }  

}

void Merge(int A[],int l,int m,int r)
{
	int *aux=new int[r-l+1];
	int i=l;
	int j=m+1;
	int k=0;
	
	while(i<=m && j<=r)
	{
		if(A[i]<=A[j]){
			aux[k++]=A[i++];
		}
		else{
			aux[k++]=A[j++];
		}
	}
	
	while(i<=m)
	{
		aux[k++]=A[i++];
	}
	while(j<=r)
	{
		aux[k++]=A[j++];
	}
	for( k = 0, i = l; i <= r; ++i, ++k )
	{
		A[ i ] = aux[ k ];
	}
        
}

void llenarRand(int A[],int n)
{
	for(int i=0;i< n;i++)
	{
		A[i]=rand()%100 + n;
	}
}
void copiar(int A[],int B[],int n)
{
	for(int i=0;i<n;i++)
	{
		B[i]=A[i];
	}
}
int main()
{
	int ram1,ram2;
	fstream fs("guardados.csv");
	srand(time(NULL));
	fs<<"tam"<<";"<<"insertSort"<<";"<<"Merge"<<";"<<"ram_insert"<<";"<<"ram_merge"<<endl;
	for(int i=1;i<1001;i++)
	{
		int n=i;
		float t0=0.0,t1=0.0,t2=0.0,t3=0.0,t4=0.0,t5=0.0;
		double time=0.0,time1=0.0,time2=0.0;
		t0=clock();
		int *A=new int[n];
		int *B=new int[n];
		int *C=new int[n];
		int *D=new int[n];
		llenarRand(A,n);
		inSort(A,n);
		t1=clock();
		time=double(t1-t0)/CLOCKS_PER_SEC;

		copiar(A,B,n);
		copiar(A,C,n);
		
		t2=clock();
		inSort(B,n);
		t3=clock();
		ram1=mem_total();
		time1=double(t3-t2) /CLOCKS_PER_SEC;
		time1=time1*1000;
		cout<<"Tiempo del insertion sort: "<<time1<<endl;

		t4=clock();

		TopDownMergeSort(C,D,n);
		t5=clock();
		ram2=mem_total();
		time2=double(t5-t4) /CLOCKS_PER_SEC;
		time2=time2*1000;
		cout<<"Tiempo del Merge Sort: "<<time2<<endl;

		if(time1<time2)
		{
			fs<<i<<";"<<time1<<";"<<time2<<";"<<ram1<<";"<<ram2<<endl;
		}
	}
	fs.close();
}
