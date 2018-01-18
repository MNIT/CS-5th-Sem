#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


typedef struct matrix
{
	int r1,c1,r2,c2;
	int x[50][50];
	int y[50][50];
} matrices;


void* multiply(void *matrix)
{
	matrices *t = (matrices*)matrix;

	int i,j,k;
	int z[t->r1][t->c2];

	for(i=0; i<t->r1; i++)
	{
		for(j=0; j<t->c1; j++)
		{
			z[i][j]=0;

			for(k=0; k<t->c2; k++)
				z[i][j] += (t->x[i][k] * t->y[k][j]);
		}
	}

	printf("\nProduct of the 2 matrices is: \n");

	for(i=0; i<t->r1; i++)
	{
		for(j=0; j<t->c2; j++)
		{
			printf("%d ", z[i][j]);
		}
		printf("\n");
	}
}

int main()
{
    matrices m;

	int r1,c1,r2,c2;
	int i,j;
    pthread_t tid;

	printf("Matrix 1\n");
	printf("rows: ");
	scanf("%d",&m.r1);
	printf("cols: ");
    scanf("%d",&m.c1);

    printf("\nMatrix 2\n");
	printf("rows: ");
	scanf("%d",&m.r2);
	printf("cols: ");
    scanf("%d",&m.c2);

	if(m.c1 != m.r2)
	{
		printf("Cannot multiply matrix\n");
		exit(-1);
	}

	printf("\nEnter data for matrix 1\n");
	for(i=0; i<m.r1; i++)
	{
		for(j=0; j<m.c1; j++)
		{
		    printf("x[%d][%d]: ", i,j);
			scanf("%d", &m.x[i][j]);
		}
	}

	printf("\nEnter data for matrix 2\n");
	for(i=0; i<m.r2; i++)
	{
		for(j=0; j<m.c2; j++)
		{
		    printf("y[%d][%d]: ", i,j);
			scanf("%d", &m.y[i][j]);
		}
	}

	pthread_create(&tid, NULL, multiply, &m);
	pthread_join(tid,NULL);
}


