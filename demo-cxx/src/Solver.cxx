/**
 * \file    Solver.cxx
 * \author  akirby
 *
 * \brief   Solver class implementation
 */

/* header files */
#include "Solver.hxx"

/* ============= */
/* Class Methods */
/* ============= */
void Solver_t::init(){
    /* set up solver on CPU -- e.g., set data in physics/numerics/etc. */
    //initialize(&solver_data);
}

void Solver_t::setupDeviceAndKernels(){
    if (!CPU_SOLVER) {
        if(rank==0) printf(" Compiling OCCA kernels...\n");

        /* set device define variables */
        if(platform.device.mode() == "HIP") platform.props["defines/HIP_MODE"] = 1;
        if(platform.device.mode() == "CUDA") platform.props["defines/CUDA_MODE"] = 1;
        if(platform.device.mode() == "OpenCL") platform.props["defines/OpenCL_MODE"] = 1;
        if(platform.device.mode() == "OpenMP") platform.props["defines/OpenMP_MODE"] = 1;
        if(platform.device.mode() == "Serial") platform.props["defines/Serial_MODE"] = 1;

        if (platform.device.mode() == "Serial") {
            platform.props["compiler_flags"] += "-O0 ";
            platform.props["compiler_flags"] += "-g "; //debugging
            platform.props["defines/OCCA_USE_SERIAL"] = 1;
        }

        if (platform.device.mode() == "CUDA") { // add backend compiler optimization for CUDA
            platform.props["compiler_flags"] += "--ftz=true ";
            platform.props["compiler_flags"] += "--prec-div=false ";
            platform.props["compiler_flags"] += "--prec-sqrt=false ";
            platform.props["compiler_flags"] += "--use_fast_math ";
            platform.props["compiler_flags"] += "--fmad=true "; // compiler option for cuda
	    platform.props["compiler_flags"] += "-arch=sm_80 "; // overrisde the cuda compute
            platform.props["compiler_flags"] += "-Xptxas -dlcm=ca";
            platform.props["defines/OCCA_USE_CUDA"] = 1;
        }

        if (platform.device.mode() == "OpenCL") { // add backend compiler optimization for OPENCL
            platform.props["compiler_flags"] += " -cl-std=CL2.0 ";
            platform.props["compiler_flags"] += " -cl-strict-aliasing ";
            platform.props["compiler_flags"] += " -cl-mad-enable ";
            platform.props["compiler_flags"] += " -cl-no-signed-zeros ";
            platform.props["compiler_flags"] += " -cl-unsafe-math-optimizations ";
            platform.props["compiler_flags"] += " -cl-fast-relaxed-math ";
            platform.props["defines/OCCA_USE_OPENCL"] = 1;
        }

        if(platform.device.mode() == "HIP"){ // add backend compiler optimization for HIP
            platform.props["compiler_flags"] += " -O3 ";
            platform.props["compiler_flags"] += " -ffp-contract=fast ";
            platform.props["compiler_flags"] += " -funsafe-math-optimizations ";
            platform.props["compiler_flags"] += " -ffast-math ";
            platform.props["defines/OCCA_USE_HIP"] = 1;
        }

        /* Platform DEFINES */
        platform.props["defines/OKL_INLINE_MACROS"] = 1;

        /* initialize solver device kernels (execute BEFORE building kernels) */
        int test_function_type_example = 3;
        initialize_solver_functions_device(test_function_type_example);

        /* convert kernel functions to macros (perform before building kernels */
        if(rank==0) initialize_solver_functions_macros();
        MPI_Barrier(comm);

        /* NOTE: SOLVER_DIR is set as a CPP defined variable */
        /* device kernels */
        norm2         = platform.buildKernel(SOLVER_DIR "/okl/linearAlgebra.okl","norm2",platform.props);
        axpby         = platform.buildKernel(SOLVER_DIR "/okl/linearAlgebra.okl","axpby",platform.props);
        vec_scale     = platform.buildKernel(SOLVER_DIR "/okl/linearAlgebra.okl","vector_scale",platform.props);
        inner_product = platform.buildKernel(SOLVER_DIR "/okl/linearAlgebra.okl","inner_product",platform.props);
      //single2double = platform.buildKernel(SOLVER_DIR "/okl/linearAlgebra.okl","single2double",platform.props);
      //double2single = platform.buildKernel(SOLVER_DIR "/okl/linearAlgebra.okl","double2single",platform.props);

        test_okl = platform.buildKernel(SOLVER_DIR "/okl/test.okl","test_okl",platform.props);
        test_okl_func = platform.buildKernel(SOLVER_DIR "/okl/test.okl","test_okl_func_runtime",platform.props);
        test_okl_struct = platform.buildKernel(SOLVER_DIR "/okl/test_struct.okl","test_okl_struct",platform.props);
        addVectors = platform.buildKernel(SOLVER_DIR "/okl/addVectors.okl","addVectors",platform.props);

#define BX 16
#define BY 16
#define BDIM 256
        platform.props["defines/BX"] = BX;
        platform.props["defines/BY"] = BY;
        platform.props["defines/BDIM"] = BDIM;
        jacobi = platform.buildKernel(SOLVER_DIR "/okl/jacobi.okl","jacobi",platform.props);
        // TODO: Add compute kernels here

        // check macro generation self-assignment
        if(rank==0 && system("./util-check-macros.sh")){
            printf(">>>> ERROR in building okl macro functions: variable self-assignment found. Exiting.\n");
            exit(1);
        }
        if(rank==0) printf("done\n");

        /* allocate scratch data space */
        o_scratch = platform.malloc<Real>(scratch.length());

        /* allocate and copy (constructor) device basis pointers */
        o_physics = platform.malloc<o_physics_t>(1,&(solver_data.physics));
        o_numerics = platform.malloc<o_numerics_t>(1,&(solver_data.numerics));
    }
}

/* =============== */
/* Private Methods */
/* =============== */
void Solver_t::initialize_solver_functions_device(int test_func_type){
#ifdef _2D_
    /* set compiler pre-processor dimension define */
    platform.props["defines/_2D_"] = true;
#endif
    /* ---------------------------------------------------------------------- */
    /* =============== */
    /* Solver Pointers */
    /* =============== */
    switch (test_func_type) {
        case(1): platform.props["defines/test_okl_func"] = "test_okl_x"; break;
        case(2): platform.props["defines/test_okl_func"] = "test_okl_y"; break;
        case(3): platform.props["defines/test_okl_func"] = "test_okl_z"; break;
    }

    /* set Riemann function */
    //switch (numerics->riemann_type) {
    //    case(0): platform.props["defines/riemann_flux_okl"] = "riemann_lax_friedrichs_okl";  break;
    //    case(1): platform.props["defines/riemann_flux_okl"] = "riemann_roe_okl";             break;
    //    case(2): platform.props["defines/riemann_flux_okl"] = "riemann_roe_entropy_fix_okl"; break;
    //}
    /* ---------------------------------------------------------------------- */
}

void Solver_t::initialize_solver_functions_macros(){
    /* Convert kernel functions that are stored in separate files (for modularity)
     * into macro functions, which are then copied directly into kernel by the
     * c-preprocessor when the kernel is built by OCCA.
     *
     * The purpose of this is due the OCCA transpiler not being able to parse
     * multi-file kernels for attributes, e.g., @inner, @outer, @exclusive, etc.
     * Thus, we need to have macros to be automatically copied back to the file
     * containing the @kernel attribute.
     */

    const std::string okl_functions[] = {
        "jacobi_inner.okl",
    };

    /* build macro functions */
    for(auto oklf : okl_functions){
        FileHandler fileHandler("./okl/" + oklf);
        function_to_macro(fileHandler,oklf);
    }
}
Real Solver_t::resNorm2(std::size_t length,amc::deviceMemory<Real> vec,char normalize){
    int blocksize = platform.props["defines/p_blockSize"];
    int Nblock = (length + blocksize-1)/blocksize;
    Nblock = (Nblock > blocksize) ? blocksize:Nblock;

    // L2 norm down to Nblock entries
    norm2(Nblock,length,vec,o_scratch);
    o_scratch.copyTo(scratch,Nblock);

    Real norm = 0.0;
    for(int n = 0; n < Nblock; ++n) norm += scratch[n];
    return (normalize) ? sqrt(norm/(Real(length))):sqrt(norm);
}
