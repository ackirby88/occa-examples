set(CMAKE_Fortran_COMPILER "/usr/bin/gfortran")
set(CMAKE_Fortran_COMPILER_ARG1 "")
set(CMAKE_Fortran_COMPILER_ID "GNU")
set(CMAKE_Fortran_COMPILER_VERSION "9.4.0")
set(CMAKE_Fortran_COMPILER_WRAPPER "")
set(CMAKE_Fortran_PLATFORM_ID "")
set(CMAKE_Fortran_SIMULATE_ID "")
set(CMAKE_Fortran_COMPILER_FRONTEND_VARIANT "")
set(CMAKE_Fortran_SIMULATE_VERSION "")




set(CMAKE_AR "/usr/bin/ar")
set(CMAKE_Fortran_COMPILER_AR "/usr/bin/gcc-ar-9")
set(CMAKE_RANLIB "/usr/bin/ranlib")
set(CMAKE_Fortran_COMPILER_RANLIB "/usr/bin/gcc-ranlib-9")
set(CMAKE_COMPILER_IS_GNUG77 1)
set(CMAKE_Fortran_COMPILER_LOADED 1)
set(CMAKE_Fortran_COMPILER_WORKS TRUE)
set(CMAKE_Fortran_ABI_COMPILED TRUE)

set(CMAKE_Fortran_COMPILER_ENV_VAR "FC")

set(CMAKE_Fortran_COMPILER_SUPPORTS_F90 1)

set(CMAKE_Fortran_COMPILER_ID_RUN 1)
set(CMAKE_Fortran_SOURCE_FILE_EXTENSIONS f;F;fpp;FPP;f77;F77;f90;F90;for;For;FOR;f95;F95)
set(CMAKE_Fortran_IGNORE_EXTENSIONS h;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_Fortran_LINKER_PREFERENCE 20)
if(UNIX)
  set(CMAKE_Fortran_OUTPUT_EXTENSION .o)
else()
  set(CMAKE_Fortran_OUTPUT_EXTENSION .obj)
endif()

# Save compiler ABI information.
set(CMAKE_Fortran_SIZEOF_DATA_PTR "8")
set(CMAKE_Fortran_COMPILER_ABI "")
set(CMAKE_Fortran_LIBRARY_ARCHITECTURE "x86_64-linux-gnu")

if(CMAKE_Fortran_SIZEOF_DATA_PTR AND NOT CMAKE_SIZEOF_VOID_P)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_Fortran_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_Fortran_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_Fortran_COMPILER_ABI}")
endif()

if(CMAKE_Fortran_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "x86_64-linux-gnu")
endif()





set(CMAKE_Fortran_IMPLICIT_INCLUDE_DIRECTORIES "/usr/lib/gcc/x86_64-linux-gnu/9/finclude;/opt/intel/oneapi/tbb/2021.3.0/include;/opt/intel/oneapi/mpi/2021.3.0/include;/opt/intel/oneapi/dev-utilities/2021.3.0/include;/opt/intel/oneapi/compiler/2021.3.0/linux/include;/opt/intel/compilers_and_libraries_2020.2.254/linux/ipp/include;/opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/include;/opt/intel/compilers_and_libraries_2020.2.254/linux/pstl/include;/opt/intel/compilers_and_libraries_2020.2.254/linux/pstl/stdlib;/opt/intel/compilers_and_libraries_2020.2.254/linux/tbb/include;/opt/intel/compilers_and_libraries_2020.2.254/linux/daal/include;/usr/lib/gcc/x86_64-linux-gnu/9/include;/usr/local/include;/usr/include/x86_64-linux-gnu;/usr/include")
set(CMAKE_Fortran_IMPLICIT_LINK_LIBRARIES "gfortran;m;gcc_s;gcc;quadmath;m;gcc_s;gcc;c;gcc_s;gcc")
set(CMAKE_Fortran_IMPLICIT_LINK_DIRECTORIES "/opt/intel/oneapi/mpi/2021.3.0/libfabric/lib;/opt/intel/oneapi/mpi/2021.3.0/lib;/opt/intel/oneapi/compiler/2021.3.0/linux/lib;/opt/intel/compilers_and_libraries_2020.2.254/linux/mpi/intel64/libfabric/lib;/usr/lib/gcc/x86_64-linux-gnu/9;/usr/lib/x86_64-linux-gnu;/usr/lib;/lib/x86_64-linux-gnu;/lib;/opt/intel/oneapi/tbb/2021.3.0/lib/intel64/gcc4.8;/opt/intel/oneapi/mpi/2021.3.0/lib/release;/opt/intel/oneapi/compiler/2021.3.0/linux/compiler/lib/intel64_lin;/opt/intel/oneapi/clck/2021.3.0/lib/intel64;/opt/intel/compilers_and_libraries_2020.2.254/linux/ipp/lib/intel64;/opt/intel/compilers_and_libraries_2020.2.254/linux/compiler/lib/intel64_lin;/opt/intel/compilers_and_libraries_2020.2.254/linux/mkl/lib/intel64_lin;/opt/intel/compilers_and_libraries_2020.2.254/linux/tbb/lib/intel64/gcc4.8;/opt/intel/compilers_and_libraries_2020.2.254/linux/daal/lib/intel64_lin;/opt/intel/compilers_and_libraries_2020.2.254/linux/tbb/lib/intel64_lin/gcc4.8")
set(CMAKE_Fortran_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")
