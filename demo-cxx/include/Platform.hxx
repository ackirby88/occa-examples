/**
 * File:   Platform.hxx
 * Author: akirby
 *
 * Created on September 29, 2021, 2:50 PM
 */

#ifndef PLATFORM_HXX
#define PLATFORM_HXX

/* system header files */
#include <mpi.h>
#include <occa.hpp>
#include <unistd.h>
#include <cstring>

#ifdef __DG_CUDA__
#  include <cuda_runtime.h>
#endif

#ifdef __DG_HIP__
#  include <hip/hip_runtime.h>
#endif

/* header files */
#include "Memory.hxx"
#include "colors.h"
#include "precision_types.h"

#define CPU_MODE    0
#define HIP_MODE    1
#define CUDA_MODE   2
#define OPENCL_MODE 3
#define OPENMP_MODE 4
#define DPCPP_MODE  5
#define METAL_MODE  6
#define SERIAL_MODE 7

class Platform {
  public:
    MPI_Comm comm;
    FILE *log_io;

    occa::json props;
    occa::device device;

    int rank,nrank;
    int thread_model;

    /* constructors */
    Platform(FILE *_logfile,MPI_Comm _comm,int _thread_model,int device_id=0,int device_platform=0):
        log_io(_logfile),comm(_comm),thread_model(_thread_model)
    {
        MPI_Comm_rank(_comm, &rank);
        MPI_Comm_size(_comm, &nrank);

        if(rank==0){
            DGOUT(log_io,"Thread Model: %s, device_id: %d (Rank 0)",
                  thread_model == CPU_MODE    ? "CPU":
                  thread_model == HIP_MODE    ? "HIP":
                  thread_model == CUDA_MODE   ? "CUDA":
                  thread_model == OPENCL_MODE ? "OpenCL":
                  thread_model == OPENMP_MODE ? "OpenMP":
		  thread_model == DPCPP_MODE  ? "DPCPP":
                  thread_model == METAL_MODE  ? "Metal":
		                                "Serial",
                  device_id);
            if(thread_model == OPENCL_MODE){
                DGOUT(log_io,", platform: %d\n",device_platform);
            } else {
                DGOUT(log_io,"\n");
            }
        }
        DeviceConfig(thread_model,device_id,device_platform);
    }
   ~Platform(){}

    /* methods */
    occa::kernel buildKernel(const std::string &fileName,
                             const std::string &kernelName,
                             const occa::json  &props = occa::json()){
        if(!rank) std::cout << "   Kernel: " << kernelName << std::endl;
        occa::kernel kernel;

        /* build on root first */
        if(!rank) kernel = device.buildKernel(fileName,kernelName,props);
        MPI_Barrier(comm);

        /* remaining ranks find the cached version (ideally) */
        if(rank) kernel = device.buildKernel(fileName,kernelName,props);
        MPI_Barrier(comm);
        return kernel;
    }

    template <class T>
    amc::deviceMemory<T> malloc(const occa::dim_t count,
                               const void *src = NULL,
                               const occa::json &prop = occa::json()) {
     if (occa::dtype::get<T>() == occa::dtype::none) {
        return amc::deviceMemory<T>(device.malloc(count*sizeof(T), occa::dtype::byte, src, prop));
      } else {
        return device.malloc<T>(count, src, prop);
      }
    }

    template <class T>
    amc::deviceMemory<T> malloc(const occa::dim_t entries,
                               const occa::memory &src,
                               const occa::json &prop = occa::json()) {
        return device.malloc<T>(entries, src, prop);
    }

    template <class T>
    amc::deviceMemory<T> malloc(const occa::dim_t entries,
                               const occa::json &prop) {
        return device.malloc<T>(entries, prop);
    }

    template <typename T>
    amc::deviceMemory<T> malloc(const amc::memory<T> src,
                               const occa::json &prop = occa::json()) {
      if (occa::dtype::get<T>() == occa::dtype::none) {
        return amc::deviceMemory<T>(device.malloc(src.byte_size(), occa::dtype::byte, src.ptr(), prop));
      } else {
        return amc::deviceMemory<T>(device.malloc<T>(src.length(), src.ptr(), prop));
      }
    }

    void *hostMalloc(const size_t bytes,
                     const void *src,
                     occa::memory &h_mem){
        occa::properties hostProp;
        hostProp["host"] = true;
        h_mem = device.malloc(bytes, src, hostProp);
        return h_mem.ptr();
    }

    void memcpyToSymbol(const void *dst,
                        const void *src,
                        const size_t bytes){
#ifdef __DG_HIP__
        if(thread_model == HIP_MODE) hipMemcpyToSymbol(dst, src, bytes);
#endif
#ifdef __DG_CUDA__
        if(thread_model == CUDA_MODE) cudaMemcpyToSymbol(dst, src, bytes);
#endif
    }

  private:
    void DeviceConfig(int thread_model,int device_id,int plat);
};

#endif /* PLATFORM_HXX */
