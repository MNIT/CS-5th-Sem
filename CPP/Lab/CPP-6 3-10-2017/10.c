#include<stdio.h>
#include<mpi.h>

#define BUFFER_SIZE 200

int main(int argc, char *argv[])
{
    int  MyRank, Numprocs;
    int  Root = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
    MPI_Comm_size(MPI_COMM_WORLD, &Numprocs);

    
    char buffer[BUFFER_SIZE];
    
    if(MyRank == 0)
    	sprintf(buffer, "Hello World");
    	
    MPI_Bcast(buffer, BUFFER_SIZE, MPI_CHAR, Root, MPI_COMM_WORLD);
   
    if(MyRank == 0)
        printf("\n Process with Rank %d broadcating MESSAGE \"%s\" to every process. \n\n", MyRank, buffer);
    else
    	printf("\n Process with Rank %d received MESSAGE \"%s\" from process 0. \n\n", MyRank, buffer);
    
    MPI_Finalize();
}




