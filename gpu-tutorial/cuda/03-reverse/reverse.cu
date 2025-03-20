#include "cuda.h"

#include <stdio.h>
#include <stdlib.h>

__global__ void reverseKernel(int N, float *d_a){
	   
  // Convert thread and thread-block indices into array index 
  const int n  = threadIdx.x + blockDim.x*blockIdx.x;

  // If index is in [0,N/2], swap values
  if(n<N/2){
    float an = d_a[n];
    d_a[n] = d_a[N-1-n];
    d_a[N-1-n] = an;
  }
}

int main(int argc,char **argv){
  int N = 31; // size of array for this DEMO
    
  float *d_a;  // Allocate DEVICE array
  cudaMalloc(&d_a, N*sizeof(float));
    
  int nThreadsPerBlock = 10;
  int nThreadBlocks = ((N/2)+nThreadsPerBlock-1)/nThreadsPerBlock;
  
  // HOST array
  float *h_a = (float*) malloc(N*sizeof(float));
  for(int i = 0; i < N; i++){
    h_a[i] = i;
    printf("h_a[%d]=%f\n",i,h_a[i]);
  }
    
  // Copy data from HOST to DEVICE
  cudaMemcpy(d_a, h_a, N*sizeof(float), cudaMemcpyHostToDevice);
   
  // Queue kernel on DEVICE
  reverseKernel <<< nThreadBlocks, nThreadsPerBlock >>> (N, d_a);
    
  // Transfer result from DEVICE to HOST
  cudaMemcpy(h_a, d_a, N*sizeof(float), cudaMemcpyDeviceToHost);
    
  // Print out result
  for(int n=0;n<N;++n){
    printf("h_a[%d] = %f\n", n, h_a[n]);
  }

  free(h_a);
  return 0;
}
