#include<stdio.h>
#include<math.h>
#include<mpi.h>

#define DIMENSION 3
#define TOTAL 8

int main(int argc, char *argv[])
{
    typedef struct 
    {
       int id;
       int result;
    } node;

  node Process;
  int MyRank, MyPartner, Size, output;
  int i, j, length;
  int RecvData;
  int Destination, Destination_tag;
  int Source, Source_tag, root;

  node pfsum_result[TOTAL];
  MPI_Status Status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

  Process.id = Process.result = output = MyRank;
  
  for(i=0; i<DIMENSION; i++)
  {
      MyPartner = Process.id ^ (int)pow(2,i);

      Destination = MyPartner;
      Destination_tag = Process.id;
      MPI_Send(&output, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);

      Source = MyPartner;
      Source_tag = MyPartner;
      MPI_Recv(&RecvData, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &Status);

      output += RecvData;
      
      if(MyPartner < Process.id)
          Process.result += RecvData;
  }

  root = 0;
  MPI_Gather(&Process, sizeof(node), MPI_CHAR, &pfsum_result, sizeof(node), MPI_CHAR, root, MPI_COMM_WORLD);
 
  if(MyRank == 0)
  {
      printf("    id     result \n");

      for (i=0; i<TOTAL; i++)
          printf("    %d       %d \n", pfsum_result[i].id, pfsum_result[i].result);
  }

  MPI_Finalize();
}
