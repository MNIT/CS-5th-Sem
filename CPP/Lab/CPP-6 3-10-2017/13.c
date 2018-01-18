#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main()
{
	int i, j, N, NoOfthreads, total_threads;

    printf("Enter number of threads: ");
    scanf("%d", &NoOfthreads);

    printf("Matrix size: ");
    scanf("%d", &N);
 
    if ((NoOfthreads!=1) && (NoOfthreads!=2) && (NoOfthreads!=4) && (NoOfthreads!=8) && (NoOfthreads!= 16) )
    {
        printf("\n Number of threads should be 1,2,4,8 or 16 for the execution of program. \n\n");
        exit(-1);
    }

	double InputMatrix[N][N];
	double CheckMatrix[N][N];

	for(i=0; i<N; i++)
		for(j=0; j<N; j++)
			InputMatrix[i][j] = CheckMatrix[i][j] = i + j;

	omp_set_num_threads(NoOfthreads);

	for(j=1; j<N; j++)
	{
		#pragma omp parallel for
		for(i = 1; i < N; i++)
        {
            if(omp_get_thread_num() == 0)
                total_threads=omp_get_num_threads();

			InputMatrix[i][j] = InputMatrix[i][j] + InputMatrix[i][j - 1];
        }
    }
         
	for(j=1; j<N; j++)
		for(i=1; i<N; i++)
			CheckMatrix[i][j] = CheckMatrix[i][j] + CheckMatrix[i][j - 1];

	for(i=0; i<N; i++)
		for(j=0; j<N; j++)
			if(CheckMatrix[i][j] == InputMatrix[i][j])
				continue;
			else
			{
				printf("\n\t\t The result of the serial and parallel calculation are not Equal \n");
				exit(1);
			}

	printf("\n The Output Matrix After Loop Nest Containing a Recurrence \n");
	
	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
			printf("%lf\t", InputMatrix[i][j]);
		printf("\n");
	}

    printf("\n\n\t\t Threads     : %d",total_threads);	
	printf("\n\n\t\t Serial And Parallel Calculation Are Same. \n");
    printf("\n\t\t..........................................................................\n");
	printf("\n");
}

