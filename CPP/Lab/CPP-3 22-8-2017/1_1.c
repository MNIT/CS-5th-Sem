#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int turn=1;

void *p(void *a)
{
	while(1)
	{
		printf("        p in non-critical section\n");
		sleep(1);
		
		while(turn != 1);
		
		printf("        p in critical section\n");
		turn = 2;
		
	}
}

void *q(void *a)
{
	while(1)
	{
		printf("        q in non-critical section\n");
		sleep(1);
		
		while(turn != 2)
		
		printf("        q in critical section\n");
		turn = 1;
		
	}
}

int main()
{
	pthread_t x,y;
	
	pthread_create(&x, NULL, p, NULL);
	pthread_create(&y, NULL, q, NULL);
	pthread_join(x, NULL);
	pthread_join(y, NULL);
}
