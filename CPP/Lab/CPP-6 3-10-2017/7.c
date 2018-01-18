#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

#define PI 3.141592653589793238462643

int main(int argc, char *argv[])
{
    int NoOfIntervals;
    int	MyRank, Numprocs;
    int iproc, i, Root = 0;
    int Source, Source_tag, Destination, Destination_tag;
    double mypi, pi, step, sum, x;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &Numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

    if(MyRank == Root)
    {
        printf("Enter the number of Intervals: ");
        scanf("%d", &NoOfIntervals);

        for(iproc=1; iproc<Numprocs; iproc++)
        {
	        Destination = iproc;
	        Destination_tag = 0;
            MPI_Send(&NoOfIntervals, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);
        }
    }
    
    else
    {
	    Source = Root;
	    Source_tag = 0;
        MPI_Recv(&NoOfIntervals, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &status);
    }

    step = 1.0/(double)NoOfIntervals;
    sum = 0.0;

    for(i=MyRank; i<NoOfIntervals; i+=Numprocs)
    {
        x = ((double)i+0.5) * step;
        sum += (4.0/(1.0 + x*x));
    }
    
    //printf("%f\n", sum);
	
    pi = 0.0;

    if(MyRank == Root)
    {
        pi = pi + (sum*step);
       
        for(iproc=1; iproc<Numprocs; iproc++)
        {
            Source = iproc;
	        Source_tag = 0;
            MPI_Recv(&mypi, 1, MPI_DOUBLE, Source, Source_tag, MPI_COMM_WORLD, &status);
            pi = pi + (mypi*step);
        }
    }

    else
    {
	    Destination = Root;
	    Destination_tag = 0;
        MPI_Send(&sum, 1, MPI_DOUBLE, Destination, Destination_tag, MPI_COMM_WORLD);
    }

    if(MyRank == Root)
        printf("\tApproximate value of pi = %.16f\n\tError = %.16f\n", pi, fabs(pi-PI));

    MPI_Finalize();
}
