#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>

int common=1;
sem_t mutex;

void fetch_and_add(int *local,int x)
{
	*local=common;
	common+=x;
}

void *p(void *ptr)
{
	int local1=0;
	int x=0;
	while(1)
	{
		printf("P : Non-critical section\n");
		sleep(1);
		
		do
		{		
			fetch_and_add(&local1,1);
			sleep(rand()%2);
			x++;
		} while(local1!=1);
		
		printf("P : Critical section \n");
		sleep(1);
		
		sem_wait(&mutex);
			fetch_and_add(&local1,-common);
		sem_post(&mutex);
		
		printf("P : Coming out from critical section \n");
		sleep(1);
	}
}

void *q(void *ptr)
{
	int local2=0;
	int x=0;
	while(1)
	{
		printf("Q : Non-critical section\n");
		sleep(1);
		
		do
		{
			fetch_and_add(&local2,1);
			sleep(rand()%3);
			x++;
		} while(local2!=1);
		
		printf("Q : Enters critical section \n");
		sleep(1);
		
		sem_wait(&mutex);
			fetch_and_add(&local2,-common);
		sem_post(&mutex);
		
		printf("Q : Coming out from critical section \n");
		sleep(1);
	}
}

int main()
{
	sem_init(&mutex, 0, 1);
	
	pthread_t tid[2];
	pthread_create(&tid[0],NULL,(void*)&p,NULL);
	pthread_create(&tid[1],NULL,(void*)&q,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
}





