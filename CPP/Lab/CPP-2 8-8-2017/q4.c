#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>

int main()
{
	int i;
	
	while(1)
	{
		printf("%d\n", getpid());
		sleep(1);
	}
}
