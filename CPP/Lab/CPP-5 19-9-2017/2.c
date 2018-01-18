#include<stdio.h>
#include<pthread.h>

int forks[5] = {2, 2, 2, 2, 2};
pthread_mutex_t monitor;
pthread_cond_t okToEat[5];

void takeForks(int n)
{
	pthread_mutex_lock(&monitor);
		
		printf("     Philosopher %d : Waiting for forks.\n", n+1);
		
		if(forks[n] != 2)
			pthread_cond_wait(&okToEat[n], &monitor);
		
		//left
		if(n != 0)
			forks[n-1]--;
		else
			forks[4]--;
		
		//right
		forks[(n+1)%5]--;
		
		printf("     Philosopher %d : Got the forks.\n", n+1);
		printf("     Philosopher %d : [ ", n+1);
		int i;
		for(i=0; i<5; i++)
			printf("%d ", forks[i]);
		printf("]\n");
		
		sleep(1);
		
	pthread_mutex_unlock(&monitor);
	//sleep(1);
}

void releaseForks(int n)
{
	pthread_mutex_lock(&monitor);		
		
		//left
		if(n==0)
			forks[4]++;
		else
			forks[n-1]++;
		
		//right
		forks[(n+1)%5]++;
		
		printf("     Philosopher %d : Released forks\n", n+1);
		printf("     Philosopher %d : [ ", n+1);
		int i;
		for(i=0; i<5; i++)
			printf("%d ", forks[i]);
		printf("]\n");
				
		//left philosopher has 2 forks
		if(n != 0)
		{
			if(forks[n-1] == 2)
			{
				printf("#### Philosopher %d signled Philosopher %d.\n", n+1, n);
				pthread_cond_signal(&okToEat[n-1]);
			}
		}
		else
		{
			if(forks[4] == 2)
			{
				printf("#### Philosopher %d signled Philosopher %d.\n", 1, 5);
				pthread_cond_signal(&okToEat[4]);
			}
		}
		
		//right philosopher has 2 forks
		if(forks[(n+1)%5] == 2)
		{
			printf("#### Philosopher %d signled Philosopher %d.\n", n+1, n+2<6?n+2:1 );
			pthread_cond_signal(&okToEat[(n+1)%5]);
		}
		
		sleep(1);
		
	pthread_mutex_unlock(&monitor);
	//sleep(1);
}

void *dining(void *arg)
{
	int n = (int)arg;
	
	while(1)
	{
		printf("     Philosopher %d : Thinking.\n", n+1);
		sleep(1);
		
		takeForks(n);
		printf("-----Philosopher %d : ----EATING----.\n", n+1);
		sleep(1);
		releaseForks(n);
		
		sleep(1);
	}
}

int main()
{
	pthread_t tid[5];	
	int i;
	
	pthread_mutex_init(&monitor,NULL);

	for(i=0; i<5; i++)
		pthread_cond_init(&okToEat[i],NULL);
	
	for(i=0; i<5; i++)
		pthread_create(&tid[i], NULL, dining, (void*)i);
	
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}








