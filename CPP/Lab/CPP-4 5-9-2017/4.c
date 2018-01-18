#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>

sem_t forks[5];

void *eat(void *arg)
{
	int n = (int)arg;
	
	while(1)
	{
		printf("     Philosopher %d : Thinking.\n", n+1);
		sleep(1);
		
		printf("     Philosopher %d : Waiting for left fork %d.\n", n+1, n+1);
		sem_wait(&forks[n]);
		printf("     Philosopher %d : Got left fork %d.\n", n+1, n+1);
		
		printf("     Philosopher %d : Waiting for right fork %d.\n", n+1, (n+2)>5?(n+2)%5:n+2);
		sem_wait(&forks[(n+1)%5]);
		printf("     Philosopher %d : Got right fork %d.\n", n+1, (n+2)>5?(n+2)%5:n+2);
		
		printf("-----Philosopher %d : ----EATING----.\n",n+1);
		sleep(1);
		
		printf("     Philosopher %d : Freed left fork %d.\n", n+1, n+1);		
		sem_post(&forks[n]);
		sleep(1);
		
		printf("     Philosopher %d : Freed right fork %d.\n", n+1, (n+2)>5?(n+2)%5:n+2);		
		sem_post(&forks[(n+1)%5]);
	}
}

int main()
{
	int i;
	
	for(i=0; i<5; i++)
		sem_init(&forks[i],0,1);
		
	pthread_t philosophers[5];
	
	for(i=0; i<5; i++)
		pthread_create(&philosophers[i], NULL, eat, (void*)i);
		
	for(i=0; i<5; i++)
		pthread_join(philosophers[i], NULL);
	
	return 0;
}








