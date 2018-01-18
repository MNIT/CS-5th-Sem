#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>

pthread_cond_t notZero;
pthread_mutex_t mutex;
int s;

void wait()
{    
    pthread_mutex_lock(&mutex);
		while(s==0)
			pthread_cond_wait(&notZero, &mutex);
		s = s - 1;
	pthread_mutex_unlock(&mutex);
}

void signal()
{    
    pthread_mutex_lock(&mutex);
		s=s+1;
		pthread_cond_signal(&notZero);
    pthread_mutex_unlock(&mutex);
}

void *p(void *arg)
{	
	while(1)
	{
		printf("    Process P is in NCS\n");
		sleep(2);
		
		wait();
			
		printf("    Process P is in CS\n");
		sleep(1);
		
		signal();
		printf("    Process P coming out from CS\n");
		sleep(1);
	}
	
	pthread_exit(NULL);
}

void *q(void *arg)
{	
	while(1)
	{
		printf("    Process Q is in NCS\n");
		sleep(1);
		
		wait();
		
		printf("    Process Q is in CS\n");
		sleep(1);
		
		signal();
		printf("    Process Q coming out from CS\n");
		sleep(2);
	}
	
	pthread_exit(NULL);
}

void main()
{
	s=1;
	
    pthread_t tid[2];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&notZero, NULL);
       
    pthread_create(&tid[0], NULL, p, NULL);
    pthread_create(&tid[1], NULL, q, NULL);
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
}











