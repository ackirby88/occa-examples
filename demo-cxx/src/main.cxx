/** Top-level main function.
 * \file    main.cxx
 * \author  akirby
 * \brief   Main function.
 */

/* header files */
#include "main.hxx"

/** Main
 * @param [in] argc     number of command line arguments
 * @param [in] argv     command line arguments
 * @return              program status
 */
int main(int argc,char **argv){
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank,nrank,ret;
    Real val;

    /* ================================ */
    /*           DEVICE MODES           */
    /*     platform to run solver       */
    /* ================================ */
    /*    CPU_MODE (CPU ONLY)           *
     * Serial_MODE (OCCA: serial CPU)   *
     *    HIP_MODE (OCCA: AMD GPU)      *
     *   CUDA_MODE (OCCA: NVIDIA GPU)   *
     *  DPCPP_MODE (OCCA: INTEL GPU)    *
     * OPENCL_MODE (OCCA: GPU)          *
     * OPENMP_MODE (OCCA: parallel CPU) *
     * ================================ */
    int device_mode = CUDA_MODE; // TODO -- CHANGE ME to test different modes!
    int device_id = 0;
    int device_platform = 0;

    /* display all devices available to OCCA */
    occaPrintModeInfo();

    /* ====================================================================== */
    /*                             INITIALIZATION                             */
    /* ====================================================================== */
    /* initialize MPI */
    ret = initialize_mpi(argc,argv,
                         comm,&rank,&nrank);

    /* initialize solver on host and device */
    Solver_t *solver = initialize_solver(comm,rank,device_mode,device_id,device_platform);
    Platform &platform = solver->platform;

    int p_blockSize = platform.props["defines/p_blockSize"];
    int nentries = 16*p_blockSize;

    /* ====================================================================== */
    /*                                 TEST #1                                */
    /* ====================================================================== */
    occa::memory d_val = platform.malloc<Real>(1); // allocate device memory

    // device function execution
    solver->test_okl(d_val);

    // copy device data to host
    d_val.copyTo(&val,1);
    printf(" Value from test_okl: %f\n",val);

    /* ====================================================================== */
    /*                                 TEST #2                                */
    /* ====================================================================== */
    occa::memory d_val_array = platform.malloc<Real>(nentries); // allocate device memory
    amc::memory<Real> val_array(nentries); // allocate host smemory

    // device function execution
    solver->test_okl_func(nentries,d_val_array);

    // copy device data to host
    d_val_array.copyTo(val_array.ptr(),val_array.size());
    printf(" Value from test_okl_func: %f\n",val_array[nentries-1]);

    /* ====================================================================== */
    /*                                 TEST #3                                */
    /* ====================================================================== */
    amc::memory<Real> a(nentries,4.0); // allocate host memory & initialize value
    amc::memory<Real> b(nentries,6.0); // allocate host memory & initialize value
    amc::memory<Real> ab(nentries);    // allocate host memory

//    d_a.malloc(a.length());
//    d_a.copyFrom(a.ptr());
    
    amc::deviceMemory<Real> d_a = platform.malloc<Real>(a); // allocate device memory; copy host memory
    amc::deviceMemory<Real> d_b = platform.malloc<Real>(b); // allocate device memory/initialize value
    amc::deviceMemory<Real> d_ab = platform.malloc<Real>(nentries);    // allocate device memory

    // device function execution
    solver->addVectors(nentries,d_a,d_b,d_ab);

    // copy device data to host
    d_ab.copyTo(ab,ab.size());
    printf(" Value from addVectors: %f %f\n",ab[0],ab[nentries-1]);

    /* ====================================================================== */
    /*                                 TEST #4                                */
    /* ====================================================================== */

    std::cout << " =============================================\n";
    std::cout << " Final Device Memory Allocated (MB): "
              << (platform.device.memoryAllocated()/1024./1024.)
              << std::endl;
    std::cout << " =============================================\n";
    return ret;
}
