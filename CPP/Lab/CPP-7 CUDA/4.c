
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cuda.h>

#define BLOCKSIZE 16

double gettime();
void sprand(int nrows, int ncols, int nnz, int row_ptr[], int col_idx[], float matrix[]);
void*  malloc_safe_call(int size);

/************************************************************************************
*  pragma routine to report the detail of cuda error
************************************************************************************/
#define CUDA_SAFE_CALL(call)                                                    \
            do{                                                                 \
                cudaError_t err = call;                                         \
                if(err != cudaSuccess)                                          \
                 {                                                              \
                   fprintf(stderr, "Cuda error in file '%s' in line %i : %s.\n",\
                   __FILE__, __LINE__, cudaGetErrorString( err));        	\
                   exit(1);                                                     \
                 }                                                              \
             } while (0)                                                        \


//kernel that performs sparse_matrix vector multiplication
__global__ void CudaSparseMatrixVectorMultiply(float* dev_matrix, float* dev_vector, int* dev_row_ptr, int* dev_col_idx, float* dev_result, int nrows, int threadDim)
{
	int i;
	int tidx = threadIdx.x;
	int tidy = threadIdx.y;
	int tindex = (threadDim * tidx) + tidy;
	int maxNumThreads = threadDim * threadDim;
	int thread_rownum;
	int pass = 0;

	if((thread_rownum  =  (tindex + maxNumThreads * pass))< nrows)
	{
		dev_result[thread_rownum] = 0;

		for(i = dev_row_ptr[thread_rownum]; i< dev_row_ptr[thread_rownum+1]; i++)
		{
			dev_result[thread_rownum] += dev_matrix[i] * dev_vector[dev_col_idx[i]];
		}
		pass++;
	}

	__syncthreads();
}


int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("USAGE: <./program_name> <no. of rows> <sparsity in the range of 0 to 1>\n");
		exit(-1);
	}

	int n = atoi(argv[1]);		//size of vector
	float sparsity = atof(argv[2]); //percentage of zero's
	int nnz = n*n/(1 - sparsity);	//number of non-zeros's
	int i;

	//host vectors
	int *row_ptr;			//row vector of the sparse matrix
	int *col_idx;			//column index vector of the sparse matrix
	float *matrix,*vector, *result;

	//device vectors
	int *dev_row_ptr;
	int *dev_col_idx;
	float *dev_matrix, *dev_vector,*dev_result;

	row_ptr   = (int *)malloc_safe_call(sizeof(int) * (n+1));
	col_idx   = (int *)malloc_safe_call(sizeof(int) * nnz);
	matrix 	  = (float *)malloc_safe_call(sizeof(float) * nnz);

	//generate the sparse martrix and row_ptr and col_idx vectors
	sprand(n,n,nnz,row_ptr,col_idx,matrix);

	vector  = (float *)malloc_safe_call(sizeof(float) * n);
	result  = (float *)malloc_safe_call(sizeof(float) * n);

	//assign values to the vector used in multiplication
	for(i = 0; i<n; i++)
	{
		vector[i] = (rand() / (float) RAND_MAX > 0.5);
	}

	//allocate memory on devie
	CUDA_SAFE_CALL(cudaMalloc((void **) &dev_matrix,sizeof(float)*nnz));
	CUDA_SAFE_CALL(cudaMalloc((void **) &dev_row_ptr,sizeof(float)*(n+1)));
	CUDA_SAFE_CALL(cudaMalloc((void **) &dev_col_idx,sizeof(float)*nnz));
	CUDA_SAFE_CALL(cudaMalloc((void **) &dev_vector,sizeof(float)*n));
	CUDA_SAFE_CALL(cudaMalloc((void **) &dev_result,sizeof(float)*n));

	//copy host vectors to device
	CUDA_SAFE_CALL(cudaMemcpy(dev_vector,vector,sizeof(float)*n,cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(dev_matrix,matrix,sizeof(float)*nnz,cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(dev_row_ptr,row_ptr,sizeof(float)*(n+1),cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(dev_col_idx,col_idx,sizeof(float)*nnz,cudaMemcpyHostToDevice));

	//defining thread grid and block
	dim3 dimGrid(1,1);
	dim3 dimBlock(BLOCKSIZE, BLOCKSIZE);

	//start timing computation
	double t0 = gettime();

	//call sparse_matrix vector multiplication cudpp library function
	CudaSparseMatrixVectorMultiply<<<dimGrid, dimBlock>>>(dev_matrix, dev_vector, dev_row_ptr, dev_col_idx, dev_result, n, BLOCKSIZE);

	//stop timing computation
	double t1 = gettime();
	printf("time taken for computation is %lf seconds\n", t1-t0);

	CUDA_SAFE_CALL(cudaMemcpy(result,dev_result,sizeof(float)*n,cudaMemcpyDeviceToHost));

	cudaFree(dev_result);
	cudaFree(dev_vector);
	cudaFree(dev_matrix);
	cudaFree(dev_row_ptr);
	cudaFree(dev_col_idx);

	free(result);
	free(vector);
	free(matrix);
	free(col_idx);
	free(row_ptr);

	return 0;
}


void sprand(int nrows,int ncols,int nnz,int row_ptr[], int col_idx[],float matrix[])
{
	int i,j,k,n;
	double r;

	n = nrows * ncols;
	k = 0;

	for(i = 0; i < nrows; i++)
	{
		row_ptr[i] = k;

		for(j = 0; j < ncols; j++)
		{
			r = rand() / (double) RAND_MAX;

			if(r * (n - (i * ncols + j)) < (nnz - k))
			{
				col_idx[k] = j;
				matrix[k] = 1.0;
				k = k+1;
			}
		}
	}

	row_ptr[nrows] = k;
}

double gettime()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec + 1e-6*tv.tv_usec;
}

void *malloc_safe_call(int size)
{
	void *ptr;

	ptr = malloc(size);

	if(ptr==NULL)
	{
		printf("memory unavailable\n");
		exit(-1);
	}

	return(ptr);
}
