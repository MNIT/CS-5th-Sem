#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int main()
{
	float sum, serialsum;
	int n, i, k, NoOfthreads;

	printf("Enter number of threads: ");
    scanf("%d", &NoOfthreads);

    printf("array size: ");
    scanf("%d", &n);
       
    if ((NoOfthreads!=1) && (NoOfthreads!=2) && (NoOfthreads!=4) && (NoOfthreads!=8) && (NoOfthreads!= 16))
    {
        printf("\n Number of threads should be 1,2,4,8 or 16 for the execution of program. \n\n");
        exit(-1);
    } 

    float array[n], check[n];

	for(i=0; i<n; i++)
	{
		array[i] = i + 5;
		check[i] = array[i];
	}

	sum = 0.0;

	omp_set_num_threads(NoOfthreads);

	#pragma omp parallel for reduction(+ : sum)
	for(i=0; i<n; i++)
		sum = sum + array_A[i];

	serialsum = 0.0;
	for (i = 0; i < n; i++)
		serialsum = serialsum +checkarray[i];

	if(serialsum != sum)
	{
		printf("\n\n\t\t The parallel calculation of array sum is different from serial calculation \n");
		exit(-1);
	}
	else
		printf("\n\n\t\t The parallel calculation of array sum is same with serial calculation \n");

	printf("\n\t\t The SumOfElements Of The Array Using OpenMP Directives Is %f\n", sum);
	printf("\t\t The SumOfElements Of The Array By Serial Calculation Is  %f\n\n", serialsum);
}

