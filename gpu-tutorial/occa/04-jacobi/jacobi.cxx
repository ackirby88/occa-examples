#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "occa.hpp"

#define datafloat float

#define BX 16
#define BY 16
#define BDIM 256

void solve(int N, datafloat tol, datafloat *h_rhs, datafloat *h_res, datafloat *h_u, datafloat *h_u2 ){
 occa::json props;

  // Setup the platform and device IDs
  occa::device device;
  device.setup({
    {"mode", "CUDA"},
    {"device_id", 0}
  });
  std::cout << "OCCA Mode: " << device.mode() << std::endl;

  if (device.mode() == "CUDA") { // add backend compiler optimization for CUDA
    props["compiler_flags"] += "-arch=sm_80 ";
    props["defines/OCCA_USE_CUDA"] = 1;
  }

  // build kernel
  props["defines/BDIM"] = BDIM;
  props["defines/BX"] = BX;
  props["defines/BY"] = BY;

  if(sizeof(datafloat)==sizeof(float))  
    props["defines/datafloat"] = "float";
  if(sizeof(datafloat)==sizeof(double))  
     props["defines/datafloat"] = "double";

  const int iterationChunk = 100; // needs to be multiple of 2
  int iterationsTaken = 0;

  // build Jacobi kernel
  occa::kernel jacobi = device.buildKernel("jacobi.okl", "jacobi", props);

  // build partial reduction kernel
  occa::kernel partialReduce = device.buildKernel("partialReduce.okl", "partialReduce", props);

  int Nred = ((N+2)*(N+2) + BDIM-1)/BDIM; // number of blocks for partial reduction

  // build Device Arrays and transfer data from host arrays
  int gridSize = (N+2)*(N+2);
  size_t sz = gridSize*sizeof(datafloat);
  
  occa::memory c_u   = device.malloc<datafloat>(gridSize, h_u);
  occa::memory c_u2  = device.malloc<datafloat>(gridSize, h_u2);
  occa::memory c_rhs = device.malloc<datafloat>(gridSize, h_rhs);
  occa::memory c_res = device.malloc<datafloat>(gridSize, h_res);

  datafloat res;

  occa::streamTag start = device.tagStream();
  // Jacobi iteration loop
  do {
    // Call jacobi [iterationChunk] times before calculating residual
    for(int i = 0; i < iterationChunk/2; ++i){
      jacobi(N, c_rhs, c_u, c_u2);
      jacobi(N, c_rhs, c_u2, c_u);
    }
    
    // calculate norm(u-u2) with interval iterationChunk iterations
    {
      // design thread array for norm(u-u2)
      int N2 = (N+2)*(N+2);
      int Nred = (N2+BDIM-1)/BDIM;
      partialReduce(N2, c_u, c_u2, c_res);
      c_res.copyTo(h_res);
      
      res = 0;
      for(int i = 0; i < Nred; ++i)
	res += h_res[i];

      res = sqrt(res/(N2));
    }
    
    iterationsTaken += iterationChunk;
    printf("residual = %9.7e after %d steps \n", res, iterationsTaken);

  } while(res > tol);
  occa::streamTag end = device.tagStream();
  const datafloat timeTaken = device.timeBetween(start,end);

  const datafloat avgTimePerIteration = timeTaken/((datafloat) iterationsTaken);
  double GBytes = (double) (6*N*N) / (1.E6) * ((double) sizeof(datafloat));
  printf("Time                       : %9.7e sec\n" , timeTaken);
  printf("Residual                   : %9.7e\n"     , res);
  printf("Iterations                 : %d\n"        , iterationsTaken);
  printf("Average time per iteration : %5.3e ms\n"  , avgTimePerIteration);
  //printf("Bandwidth                  : %f GB, %g GB/s, %ld\n", GBytes, GBytes*iterationsTaken/timeTaken, sizeof(datafloat));

  // blocking copy of solution from device to host 
  c_u.copyTo(h_u);

}

int main(int argc, char** argv){
  // parse command line arguements
  if(argc != 3){
    printf("Usage: ./main N tol \n");
    return 0;
  }

  // Number of internal domain nodes in each direction
  const int N     = atoi(argv[1]);

  // Termination criterion || unew - u ||_2 < tol 
  const datafloat tol = atof(argv[2]);

  // Host Arrays
  datafloat *h_u   = (datafloat*) calloc((N+2)*(N+2), sizeof(datafloat));
  datafloat *h_u2  = (datafloat*) calloc((N+2)*(N+2), sizeof(datafloat));
  datafloat *h_rhs = (datafloat*) calloc((N+2)*(N+2), sizeof(datafloat));
  datafloat *h_res = (datafloat*) calloc((N+2)*(N+2), sizeof(datafloat));

  // FD node spacing
  datafloat delta = 2./(N+1);

  for(int j = 0; j < N+2; ++j){
    for(int i = 0; i < N+2; ++i){
      datafloat x = -1 + delta*i;
      datafloat y = -1 + delta*j;

      // solution is u = sin(pi*x)*sin(pi*y) so the rhs is: 
      h_rhs[i + (N+2)*j] = delta*delta*(2.*M_PI*M_PI*sin(M_PI*x)*sin(M_PI*y));
    }
  }

  // Solve discrete Laplacian
  solve(N, tol, h_rhs, h_res, h_u, h_u2);
  
  // Compute maximum error in finite difference solution and output solution
  FILE *fp = fopen("result.dat", "w");
  datafloat maxError = 0;
  for(int j = 0; j < N+2; ++j){
    for(int i = 0; i < N+2; ++i){
      datafloat x = -1 + delta*i;
      datafloat y = -1 + delta*j;
      datafloat error = fabs( sin(M_PI*x)*sin(M_PI*y) - h_u[i + (N+2)*j]);
      maxError = (error > maxError) ? error:maxError;
      fprintf(fp, "%g %g %g %g\n", x, y, h_u[i+(N+2)*j],error);
    }
  }
  fclose(fp);

  printf("Maximum absolute error     : %7.9e\n"     ,  maxError);

  // Free all the mess
  free(h_u);
  free(h_u2);
  free(h_res);
  free(h_rhs);
}
