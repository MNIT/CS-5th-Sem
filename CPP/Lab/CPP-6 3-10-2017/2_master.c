#include<stdio.h>
#include<mpi.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[])
{
    int  MyRank,Numprocs, Destination, Source, iproc;
    int  Destination_tag, Source_tag;
    int  Root = 0, len_name ;
    char proc_name[100], proc_name_root[100];
    char Message[BUFFER_SIZE];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
    MPI_Comm_size(MPI_COMM_WORLD, &Numprocs);

    for(iproc = 1; iproc<Numprocs; iproc++)
    {
        Source = iproc;
        Source_tag = 0;

        MPI_Get_processor_name(proc_name_root, &len_name);

        MPI_Recv(Message, BUFFER_SIZE, MPI_CHAR, Source, Source_tag, MPI_COMM_WORLD, &status);

        printf("\n Process with Rank %d [Node : %s] Received MESSAGE \"%s \"  from Process with Rank %d\n\n", Root, proc_name_root, Message, iproc);
    }

    MPI_Finalize();
}
