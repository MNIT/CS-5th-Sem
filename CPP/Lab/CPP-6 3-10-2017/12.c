#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main()
{
	  // omp_set_num_threads(4);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
		    printf("\n\t\t \"Hello World\" is being printed by the thread : %d\n", tid);

		    if (tid == 0)
        {
			      int nthreads = omp_get_num_threads();
			      printf("\n\t\t Master thread printing total number of threads for this execution are : %d\n", nthreads);
		    }
    }
	  
    return 0;
}

