#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main()
{
	int threadid, Noofthreads;

	// omp_set_num_threads(4);

	#pragma omp parallel private(threadid)
	{
		threadid = omp_get_thread_num();

		printf("\n\t\t My thread id is : %d\n", threadid);
	}
}