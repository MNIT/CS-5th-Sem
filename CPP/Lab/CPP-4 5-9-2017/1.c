#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t s;

void *cs(void *i)
{
	printf("%d in non-critical section\n", (int)i);
	
	sem_wait(&s);
	
	printf("%d in critical section\n", (int)i);
	
	sem_post(&s);
}

void main()
{
	sem_init(&s,0,1);
	int n,i;
	
	printf("Enter number of processes: ");
	scanf("%d", &n);
	
	pthread_t tid[n];
	
	for(i=0; i<n; i++)
		pthread_create(&tid[i], NULL, cs, (void*)i);
	
	for(i=0; i<n; i++)
		pthread_join(tid[i], NULL);
}
