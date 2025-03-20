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

  if (device.mode() == "CUDA") { // add backend compiler optimization for CUDA
    props["compiler_flags"] += "-arch=sm_80 ";
    props["defines/OCCA_USE_CUDA"] = 1;
  }
  std::cout << "OCCA Mode: " << device.mode() << std::endl;

  // build kernel
  int TILE_SIZE = 512;
  props["defines/TILE_SIZE"] = TILE_SIZE;
  occa::kernel addVectors = device.buildKernel("addVectors.okl", "addVectors", props);

  // size of array
  int N = 512*8;

  float *h_a  = new float[N];
  float *h_b  = new float[N];
  float *h_ab = new float[N];

  for (int i = 0; i < N; ++i) {
    h_a[i]  = i;
    h_b[i]  = 1 - i;
  }
  
  // Allocate memory on the device
  occa::memory o_a, o_b, o_ab;
  o_a = device.malloc<float>(N);
  o_b = device.malloc<float>(N);
  o_ab= device.malloc<float>(N);

  // Copy memory to the device
  o_a.copyFrom(h_a);
  o_b.copyFrom(h_b);

  // queue kernel
  addVectors(N, o_a, o_b, o_ab);
  
  // copy result to HOST
  o_ab.copyTo(h_ab);

  for (int i = 0; i < N; ++i) {
    if (!occa::areBitwiseEqual(h_ab[i], h_a[i] + h_b[i])) {
      throw 1;
    }
  }

  // check results from GPU
  for(int n = 0; n < N; ++n) printf("h_ab[%d] = %g\n", n, h_ab[n]);

  // Free host memory
  delete [] h_a;
  delete [] h_b;
  delete [] h_ab;

  return 0; 
}
