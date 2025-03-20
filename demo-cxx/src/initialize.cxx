/** Initialization functions.
 * \file    initialize.cxx
 * \author  akirby
 */

/* header files */
#include "initialize.hxx"

/* data header files */
#include "Solver.hxx"

/* global variable definition */
Platform *computeDevice;

int initialize_mpi(int argc,char **argv,MPI_Comm comm,int *rank,int *nrank){
    int retv = MPI_Init(&argc,&argv);
    MPI_Comm_rank(comm,rank);
    MPI_Comm_size(comm,nrank);
    return retv;
}

Solver_t* initialize_solver(MPI_Comm comm,int rank,int device_mode,
                            int device_id,int device_platform){
    Solver_t *Solver;
    char verbose = 0;

    /* construct compute platform */
    computeDevice = new Platform(stdout,comm,device_mode,device_id,device_platform);

    /* initialize solver */
    Solver = (device_mode == CPU_MODE) ? new Solver_t(comm,rank,verbose):
                                         new Solver_t(*computeDevice,comm,rank,verbose);

    Solver->init();
    Solver->setupDeviceAndKernels();

    if(rank == 0){
        std::cout << " =============================================\n";
        std::cout << " Device Memory Allocated (MB): "
                  << (computeDevice->device.memoryAllocated()/1024./1024.)
                  << std::endl;
        std::cout << " =============================================\n";
    }
    return Solver;
}
