#include<stdio.h>
#include<pthread.h>

#define MAX 10

int buffer[MAX];
int fullEntries;
int t;
pthread_mutex_t monitor;
pthread_cond_t notEmpty;
pthread_cond_t notFull;

void append(int element) 
{
     pthread_mutex_lock(&monitor);     
		 
		 if(fullEntries == MAX) 
		 	 pthread_cond_wait(&notFull, &monitor);
		    
		 buffer[t] = element;   
		 fullEntries++;
		 
		 printf("    Appended packet. Buffer size = %d. Buffer = [ ", ++t);
		 int i;
		 for(i=0; i<t; i++)
		   	printf("%d ", buffer[i]);
		 printf("]\n");
		 
		 pthread_cond_signal(&notEmpty);
		            
     pthread_mutex_unlock(&monitor);
}

int take()
{
     pthread_mutex_lock(&monitor);
		 
		 if(fullEntries == 0)   
		     pthread_cond_wait(&notEmpty, &monitor);
		              
		 int tmp = buffer[t]; 
		 t--;
		 fullEntries--;
		 
		 printf("    Removed packet. Buffer size = %d. Buffer = [ ", t);
		 int i;
		 for(i=0; i<t; i++)
		   	printf("%d ", buffer[i]);
		 printf("]\n");
		 
		 pthread_cond_signal(&notFull);
		             
     pthread_mutex_unlock(&monitor);
     
     return tmp;                
}

void *producer(void *arg)
{
	while(1)
	{
		sleep(1);
		printf("\n    Produced packet.\n");
		
		append(rand()%1000);
	}
}

void *consumer(void *arg)
{
	while(1)
	{
		int t = take();
		
		printf("    Consumed packet.\n\n");
		sleep(2);
	}
}

int main()
{
	//BoundedBuffer b;
	
	t = fullEntries = 0;
	
	pthread_t ptid, ctid;
	
	pthread_mutex_init(&monitor, NULL);
	pthread_cond_init(&notEmpty, NULL);
	pthread_cond_init(&notFull, NULL);
	
	pthread_create(&ptid, NULL, producer, NULL);
	pthread_create(&ctid, NULL, consumer, NULL);
	
	pthread_join(ptid, NULL);
	pthread_join(ctid, NULL);
	
	return 0;	
}
















