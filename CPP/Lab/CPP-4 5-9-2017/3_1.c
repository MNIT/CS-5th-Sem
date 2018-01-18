#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<strings.h>

#define MAX 10

sem_t empty;
sem_t full;
sem_t mutex;

int loops=MAX;
int buffer[MAX];
int t=0;

void put(int i)
{	
	buffer[i] = i+1;
}

int get()
{
 	return buffer[t++];
}

void *producer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {    	
		sem_wait(&empty);
		sem_wait(&mutex);
			sleep(1);
		    put(i);       
		sem_post(&mutex);
		sem_post(&full);
    }
}

void *consumer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
		sem_wait(&full);
		sem_wait(&mutex);
			sleep(1);
		    int tmp = get();
		sem_post(&mutex);
		sem_post(&empty);
        
        printf("%d\n", tmp);
    }
}

int main(int argc, char *argv[])
{
    sem_init(&empty, 0, MAX); // MAX buffers are empty to begin with...
    sem_init(&full, 0, 0);    // ... and 0 are full
    sem_init(&mutex, 0, 1);
    
    pthread_t x,y;

	pthread_create(&x, NULL, producer, NULL);
	pthread_create(&y, NULL, consumer, NULL);

    pthread_join(x, NULL);
    pthread_join(y, NULL);
    
    return 0;
}







