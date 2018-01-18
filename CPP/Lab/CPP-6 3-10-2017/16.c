#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>

int main(int argc,char **argv)
{
    const double up = 1.1 ;
    double Sn, origSn=1000.0;
    int n, NoOfthreads, N;

    printf("Enter number of threads: ");
    scanf("%d", &NoOfthreads);

    printf("array size: ");
    scanf("%d", &N);
       
    if ((NoOfthreads!=1) && (NoOfthreads!=2) && (NoOfthreads!=4) && (NoOfthreads!=8) && (NoOfthreads!= 16))
    {
        printf("\n Number of threads should be 1,2,4,8 or 16 for the execution of program. \n\n");
        exit(-1);
    } 

    double opt[N+1], dependency_opt[N+1], no_dependency_opt[N+1];
	
    Sn = 1000.0;
    for(n=0; n<=N; n++)
    {
        opt[n] = Sn;
        Sn *= up;
    }
    
   
    Sn = 1000.0;
    no_dependency_opt[0] = origSn;

    omp_set_num_threads(NoOfthreads);

    #pragma omp parallel for private(n) lastprivate(Sn)
    for(n=1; n<=N; n++)
    {
        Sn = origSn * pow(up, n);
        no_dependency_opt[n] = Sn;
    }

    Sn *= up;

    for (n=0; n<=N; ++n) 
    {
	      if((int)(opt[n]) == (int)(no_dependency_opt[n])){}
	      else
        {
	          printf("%lf != %lf\n",opt[n],no_dependency_opt[n]);
	          printf("\n\t\t Incorrect results found when Serial computation results & Parallel computation \n\t\twithout Dependency results are compared\n");
            break;
	      }
    }
    
    printf("\n\t\t..........................................................................\n");
}
	
