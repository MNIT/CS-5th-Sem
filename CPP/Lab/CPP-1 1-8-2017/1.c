#include<stdio.h>
#include<pthread.h>

#define NUM 5

int shared_variable;

void *thread(void *i)
{
	shared_variable ++;
	
	printf("Thread %d started. global variable = %d\n", (int)(i+1), shared_variable);
}

void main()
{
	pthread_t tid[NUM];
	
	shared_variable = 0;
	
	int i;
	
	for(i=0; i<NUM; i++)
	{
		int error = pthread_create(&tid[i], NULL, thread, (void*)i);
		
		if(error)
			printf("Thread %d can not be created.\n", i+1);
	
	}
	
	pthread_exit(NULL);
}
