#include<stdio.h>
#include<pthread.h>

void *sum(void *n)
{
	int i,sum=0;
	
	for(i=0; i<(int)n; i++)
		sum += i;
		
	printf("SUM = %d\n", sum);
}

int main()
{
	int n;
	
	printf("Enter number to find sum (1...N): ");
	scanf("%d", &n);
	
	pthread_t tid;
	
	int error = pthread_create(&tid, NULL, sum, (void*)n);
	pthread_join(tid, NULL);
	
	if(error)
		printf("Can not create thread for SUM.\n");
	
	pthread_exit(NULL);
	printf("Main thread exited.\n");
}
