#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int main()
{
	double serial_sum, sum;
	int n, i, threadid, tval, NoOfthreads;

	printf("Enter number of threads: ");
    scanf("%d", &NoOfthreads);

    printf("array size: ");
    scanf("%d", &n);
       
    if ((NoOfthreads!=1) && (NoOfthreads!=2) && (NoOfthreads!=4) && (NoOfthreads!=8) && (NoOfthreads!= 16))
    {
        printf("\n Number of threads should be 1,2,4,8 or 16 for the execution of program. \n\n");
        exit(-1);
    } 

    double array[n], check[n];

	for(i=0; i<n; i++)
	{
		array[i] = i * 5;
		check[i] = array[i];
	}

	sum = 0.0;

	omp_set_num_threads(NoOfthreads);
 
	#pragma omp parallel for
	for(i=0; i<n; i++) 
    {
		#pragma omp critical
		sum = sum + array[i];
    }

	serial_sum = 0.0;
	for(i=0; i<n; i++)
		serial_sum = serial_sum + check[i];

	if (serial_sum == sum)
		printf("\n\n\t\t The Serial And Parallel Sums Are Equal\n");
	else
	{
		printf("\n\\nt\t The Serial And Parallel Sums Are Unequal\n");
		exit(1);
	}

	printf("\n\t\t The SumOfElements Of The array Using OpenMP Directives Is %lf\n", sum);
	printf("\t\t The SumOfElements Of The array By Serial Calculation Is %lf\n\n", serial_sum);
    printf("\n\t\t..........................................................................\n");
}

