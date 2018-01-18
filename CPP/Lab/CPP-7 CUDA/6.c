
#include <stdio.h>
#include <cuda.h>
#include <math.h>


#define EPS 1.0e-12
#define GRIDSIZE 10
#define BLOCKSIZE 16

#define SIZE 128

double *dMatA, *dMatB;
double *hMatA, *hMatB;
double *dresult, *hresult,*CPU_Result,*host_MatC;
int   vlength , count = 0;
int   blockWidth;
cudaEvent_t start,stop;
cudaDeviceProp deviceProp;
int device_Count,size=SIZE;

__global__ void vectvectadd(double* dm1,double* dm2,double *dres,int num)
{
        int tx = blockIdx.x*blockDim.x + threadIdx.x;
        int ty = blockIdx.y*blockDim.y + threadIdx.y;
        int tindex=tx+(gridDim.x)*(blockDim.x)*ty;

        if(tindex < num)
	dres[tindex]= dm1[tindex]+dm2[tindex];
}

/* Check for safe return of all calls to the device */


void CUDA_SAFE_CALL(cudaError_t call)
{
        cudaError_t ret = call;
        //printf("RETURN FROM THE CUDA CALL:%d\t:",ret);
        switch(ret)
        {
                case cudaSuccess:
                //              printf("Success\n");
                                break;
        /*      case cudaErrorInvalidValue:
                                {
                                printf("ERROR: InvalidValue:%i.\n",__LINE__);
                                exit(-1);
                                break;
                                }
                case cudaErrorInvalidDevicePointer:
                                {
                                printf("ERROR:Invalid Device pointeri:%i.\n",__LINE__);
                                exit(-1);
                                break;
                                }
                case cudaErrorInvalidMemcpyDirection:
                                {
                                printf("ERROR:Invalid memcpy direction:%i.\n",__LINE__);
                                exit(-1);
                                break;
                                }                       */
                default:
                        {
                                printf(" ERROR at line :%i.%d' ' %s\n",__LINE__,ret,cudaGetErrorString(ret));
                                exit(-1);
                                break;
                        }
        }
}


/*
 * Fill in the vector with double precision values
 */
void fill_dp_vector(double* vec,int size)
{
        int ind;
        for(ind=0;ind <size;ind++)
                vec[ind]=drand48();
}

/*
 * Terminate and exit on errors on host memory allocation. This is called from
 * the functions which actually execute the benchmark
 */


/* checl grid and block dimensions */
void check_block_grid_dim(cudaDeviceProp devProp,dim3 blockDim,dim3 gridDim)
{

        if( blockDim.x >= devProp.maxThreadsDim[0] || blockDim.y >= devProp.maxThreadsDim[1] || blockDim.z >= devProp.maxThreadsDim[2] )
        {
                printf("\nBlock Dimensions exceed the maximum limits:%d * %d * %d \n",devProp.maxThreadsDim[0],devProp.maxThreadsDim[1],devProp.maxThreadsDim[2]);
               exit(-1);
        }

        if( gridDim.x >= devProp.maxGridSize[0] || gridDim.y >= devProp.maxGridSize[1] || gridDim.z >= devProp.maxGridSize[2] )
        {
                printf("\nGrid Dimensions exceed the maximum limits:%d * %d * %d \n",devProp.maxGridSize[0],devProp.maxGridSize[1],devProp.maxGridSize[2]);
               exit(-1);
        }
}

void mem_error(char *arrayname, char *benchmark, int len, char *type)
{

        printf("\nMemory not sufficient to allocate for array %s\n\tBenchmark : %s  \n\tMemory requested = %d number of %s elements\n",arrayname, benchmark, len, type);
        printf("\n\tAborting\n\n");
        exit(-1);
}

/*
 * Get the number of GPU devices present on the host
 */

int get_DeviceCount()
{
        int count;
        cudaGetDeviceCount(&count);
        return count;
}

void deviceQuery()
{
	 int device_Count;
        device_Count=get_DeviceCount();
        printf("\n\nNUmber of Devices : %d\n\n", device_Count);

        cudaSetDevice(0);
        int device;
        cudaGetDevice(&device);
        cudaGetDeviceProperties(&deviceProp,device);
        printf("Using device %d: %s \n", device, deviceProp.name);
}

/* Launch kernel */
void launch_kernel()
{
	 dim3 dimBlock(BLOCKSIZE, BLOCKSIZE);
         dim3 dimGrid((vlength/BLOCKSIZE*BLOCKSIZE)+1,1);
         check_block_grid_dim(deviceProp,dimBlock,dimGrid);

	 vectvectadd<<<dimGrid, dimBlock>>>(dMatA, dMatB, dresult,vlength );
}

/* Function to print gflosp rating */
double print_Gflops_rating(float Tsec,int size)
{
      //  printf("time taken is %.8f\n",Tsec);
	double gflops;
        gflops=(1.0e-9 * (( 1.0 * size )/Tsec));
        //printf("Gflops is \t%f\n",gflops);
	return gflops;
}

/* prints the result on screen */

void print_on_screen(char * program_name,float tsec,double gflops,int size,int flag)//flag=1 if gflops has been calculated else flag =0
{
        printf("\n---------------%s----------------\n",program_name);
        printf("\tSIZE\t TIME_SEC\t Gflops\n");
        if(flag==1)
        printf("\t%d\t%f\t%lf\t",size,tsec,gflops);
        else
        printf("\t%d\t%lf\t%lf\t",size,"---","---");

}

void dfree(double * arr[],int len)
{
        for(int i=0;i<len;i++)
                CUDA_SAFE_CALL(cudaFree(arr[i]));
        printf("mem freed\n");
}

/* Function to perform computation on CPU*/
void vectvect_add_in_cpu(double *A ,double *B,double *C,int size)
{
	for(int i=0;i<size;i++)
	C[i]=A[i]+B[i];
}



int main()
{
	double *array[3];

	array[0]=dMatA;
	array[1]=dMatB;
	array[2]=dresult;

	deviceQuery();

	vlength=SIZE;
	CUDA_SAFE_CALL(cudaEventCreate (&start));
        CUDA_SAFE_CALL(cudaEventCreate (&stop));

 	  //allocation host memory
   	hMatA = (double*) malloc( vlength *  sizeof(double));
   	hMatB = (double*) malloc( vlength * sizeof(double));
   	hresult = (double*) malloc( vlength * sizeof(double));

	 if(hMatA==NULL)
                mem_error("hMatA","vectvectmul",vlength,"double");

	 if(hMatB==NULL)
                mem_error("hMatB","vectvectmul",vlength,"double");

	 if(hresult==NULL)
                mem_error("hresult","vectvectmul",vlength,"double");

   	/* allocation device memory */
   	CUDA_SAFE_CALL(cudaMalloc( (void**)&dMatA, vlength * sizeof(double)));
   	CUDA_SAFE_CALL(cudaMalloc( (void**)&dMatB, vlength * sizeof(double)));
   	CUDA_SAFE_CALL(cudaMalloc( (void**)&dresult, vlength*sizeof(double)));

       	fill_dp_vector(hMatA,vlength);
	fill_dp_vector(hMatB,vlength);

    	CUDA_SAFE_CALL(cudaMemcpy((void*)dMatA, (void*)hMatA, vlength* sizeof(double) , cudaMemcpyHostToDevice ));
    	CUDA_SAFE_CALL(cudaMemcpy((void*)dMatB, (void*)hMatB, vlength* sizeof(double) , cudaMemcpyHostToDevice ));

   	/* calling device kernel */
	CUDA_SAFE_CALL(cudaEventRecord (start, 0));
	launch_kernel();
   	CUDA_SAFE_CALL(cudaEventRecord (stop, 0));
   	CUDA_SAFE_CALL(cudaEventSynchronize (stop));

	CUDA_SAFE_CALL(cudaMemcpy((void*)hresult, (void*)dresult,vlength*sizeof(double) , cudaMemcpyDeviceToHost ));
   	printf("\n ----------------------------------------------------------------------");

	/* computing elapsed time  */
	float elapsedTime;
	double Tsec;
        CUDA_SAFE_CALL(cudaEventElapsedTime(&elapsedTime, start, stop));

        Tsec = elapsedTime *1.0e-3; //time in sec now

	//calculation of Gflops
        print_Gflops_rating(Tsec,vlength);
	//vectvect_add_in_cpu(hMatA ,hMatB,host_MatC,vlength);


 	/* comparing results at cpu and gpu */
	 //relError(hresult,host_MatC,vlength);

	/* printing the result on screen */
        print_on_screen("vect vect ADDITION",Tsec, print_Gflops_rating(Tsec,vlength),size,1);



	//free the memory
   	dfree(array,3);
   	free(hMatA);
   	free(hMatB);
   	free(hresult);
}

