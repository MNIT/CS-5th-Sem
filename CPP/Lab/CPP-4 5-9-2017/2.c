#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t s1, s2;

void merging(int a[], int l1, int u1, int l2, int u2)
{
	int i,j,k;
	int t[l1+u2+1];
	for(i=l1, j=l2, k=l1; i<=u1 && j<=u2; k++)
	{
		if(a[i] >= a[j])
			t[k] = a[j++];
		else
			t[k] = a[i++];
	}
	
	while(i<=u1)
		t[k++] = a[i++];
	
	while(j<=u2)
		t[k++] = a[j++];
	
	for(i=l1; i<=u2; i++)
		a[i] = t[i];
}

 
void mergesort(int a[], int lb, int ub)
{
    if (lb < ub)
    {
        int mid = (lb+ub)/2;
		
		pthread_t tid1, tid2;
		
        mergesort(a, lb, mid);
        mergesort(a, mid+1, ub);
 
        merging(a, lb, mid, mid+1, ub);
    }
}

/*void *cs(void *i)
{
	printf("%d in non-critical section\n", (int)i);
	
	sem_wait(&s);
	
	printf("%d in critical section\n", (int)i);
	
	sem_post(&s);
}*/

void main()
{
	sem_init(&s1,0,1);
	sem_init(&s2,0,1);
	int n,i;
	
	printf("Enter no of elements: ");
	scanf("%d", &n);
	
	int a[n];
	
	for(i=0; i<n; i++)
	{
		printf("Enter %d element: ", i+1);
		scanf("%d", &a[i]);
	}
	
	mergesort(a, 0, n-1);
	
	for(i=0; i<n; i++)
		printf("%d ", a[i]);
	
	/*printf("Enter number of processes: ");
	scanf("%d", &n);
	
	pthread_t tid[n];
	
	for(i=0; i<n; i++)
		pthread_create(&tid[i], NULL, cs, (void*)i);
	
	for(i=0; i<n; i++)
		pthread_join(tid[i], NULL);*/
}
