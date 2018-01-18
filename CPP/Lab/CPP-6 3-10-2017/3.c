#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[])
{
    int iproc, MyRank, Numprocs, Root = 0;
    int recv, sum = 0;
    int	Source, Source_tag, Destination, Destination_tag;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &Numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

    if(MyRank == Root)
    {
        for(iproc=1; iproc<Numprocs; iproc++)
        {
            Source = iproc;
	        Source_tag = 0;

	        MPI_Recv(&recv, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &status);
	        sum = sum + recv;
        }
       
        printf(" Root Process (Rank = %d)\n SUM = %d\n", MyRank, sum);
    }

    else
    {
	    Destination = 0;
	    Destination_tag = 0;

	    MPI_Send(&MyRank, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

