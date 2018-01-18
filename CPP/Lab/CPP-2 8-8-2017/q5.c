#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *series(void *no)
{
	int i,sum=0;
	
	for(i=1; i<=(int)no; i++)
		sum += (i*i);
	
	printf("Sum is %d\n", sum);
}

int main()
{
	int n;
	printf("Enter numbers of terms: ");
	scanf("%d", &n);
	
	pthread_t tid;
	pthread_create(&tid, NULL, series, (void*)n);
	pthread_join(tid,NULL);
}
