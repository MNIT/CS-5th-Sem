#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[])
{
    int MyRank, Numprocs, Root = 0;
    int recv, sum = 0;
    int	Source, Source_tag, Destination, Destination_tag;
    MPI_Status  status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &Numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

    if(MyRank == Root)
    {
	    Destination = MyRank + 1;
	    Destination_tag = 0;
	    MPI_Send(&MyRank, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);
    }

    else
    {
        if(MyRank < Numprocs-1)
        {
	        Source = MyRank-1;
	        Source_tag = 0;

	        MPI_Recv(&recv, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &status);
	        
            sum  = MyRank+recv;
	        Destination = MyRank+1;
	        Destination_tag = 0;
	          
            MPI_Send(&sum, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);
	    }
	
        else
        {
	        Source = MyRank-1;
	        Source_tag = 0;
	          
            MPI_Recv(&recv, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &status);
	        sum = MyRank + recv;
	    }
    }

    if(MyRank == Root)
    {
	    Source = Numprocs-1;
	    Source_tag = 0;
	      
        MPI_Recv(&sum, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &status);
	    printf("\tRoot Process with Rank = %d\n\tFinal SUM %d\n", MyRank, sum);
    }

    if(MyRank == Numprocs-1)
    {
        Destination = 0;
        Destination_tag = 0;
        MPI_Send(&sum, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
