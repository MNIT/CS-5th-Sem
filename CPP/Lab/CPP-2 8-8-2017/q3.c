#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *file(void *fp)
{
	char *fptr = (char*)fp;

	FILE *f = fopen(fptr,"r");

	if(f == NULL)
		perror("Error: ");
	else
        printf("File successfully opened.\n");
}

int main(int *argc, char *argv[])
{
	pthread_t tid;
	pthread_create(&tid, NULL, file, argv[1]);
	pthread_join(tid,NULL);
	return 0;
}
