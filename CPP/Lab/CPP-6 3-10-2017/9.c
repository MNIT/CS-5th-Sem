#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

#define  LOG2(x)    log10((double)(x)) / log10((double) 2)
#define  IS_INT(x)  ((x) == (int)(x))?1:0

int main(int argc,char *argv[])
{
    int	MyRank, Numprocs;
    int Root = 0;
    int ilevel, value, ans;
    int	Source, Source_tag;
    int Destination, Destination_tag;
    int	Level, NextLevel;
    float NoofLevels;
    static int sum = 0;
    MPI_Status status;
    MPI_Request request[200];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &Numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

    NoofLevels = LOG2(Numprocs);

    if(!(IS_INT(NoofLevels)))
    {
        if(MyRank == Root)
	        printf("\nNumber of processors should be power of 2\n");
        MPI_Finalize();
        exit(-1);
    }

    sum = MyRank;
    Source_tag = 0;
    Destination_tag = 0;

    for(ilevel=0; ilevel<NoofLevels; ilevel++)
    {
	    Level = (int)(pow(2, ilevel));

	    if(MyRank % Level == 0)
        {
	        NextLevel = (int)(pow(2, ilevel +1));

	        if(MyRank % NextLevel == 0)
            {
	            Source = MyRank + Level;
	            MPI_Irecv(&value, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &request[ilevel]);
  	            MPI_Waitall(1, &request[ilevel], &status);
                sum = sum + value;
	       }
	       
           else
           {
	            Destination = (MyRank - Level);
	            MPI_Isend(&sum, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD,&request[ilevel]);
  	     	    MPI_Waitall(1, &request[ilevel], &status);
	       }
        }
    }

    if(MyRank == Root)
        printf("\tRoot Process Rank is %d.\n\tFinal SUM is %d.\n", MyRank, sum);

    MPI_Finalize();
}
