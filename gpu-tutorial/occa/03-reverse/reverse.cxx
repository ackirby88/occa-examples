#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <occa.hpp>

int main(int argc, char **argv){
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
  props["defines/TILE_SIZE"] = 10;

  // build kernel
  occa::kernel reverse = device.buildKernel("reverse.okl", "reverse", props);

  // size of array
  int N = 512;

  // set thread array for Jacobi iteration
  size_t sz = N*sizeof(float);

  // allocate array on HOST
  float *h_x = (float*) malloc(sz);
  for(int n = 0; n < N; ++n) h_x[n] = n;
  
  // allocate array on DEVICE (copy from HOST)
  occa::memory c_x = device.malloc<float>(N,h_x);

  // queue kernel
  reverse(N, c_x);
  
  // copy result to HOST
  c_x.copyTo(h_x);
  
  // check results from GPU
  for(int n = 0; n < N; ++n) printf("h_x[%d] = %g\n", n, h_x[n]);

  // clean up memory
  free(h_x);

  return 0; 
}
