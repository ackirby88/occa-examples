CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(Demo-F90)
enable_language(Fortran)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# force GNU linker to include all shared libraries
if (NOT APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
else ()
  set(CMAKE_MACOSX_RPATH ON)
endif ()

# set C++17 standard for OCCA compatibility
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# pickup occa install directory location (set from configure script)
set(occa_dir "" CACHE PATH "occa install directory location")

# --------------------------------------------------------------------------- #
# set OCCA directory
set(ENV{OCCA_DIR} "${occa_dir}")
message("OCCA_DIR: " $ENV{OCCA_DIR})

# --------------------------------------------------------------------------- #
# ======= #
# Defines #
# ======= #
add_definitions("-DSOLVER_DIR=\"${SOLVER_DIR}\"")
message("SOLVER Directory: ${SOLVER_DIR}")

# --------------------------------------------------------------------------- #
# =================================== #
# Library header files to be included #
#include_directories(include)
# =================================== #

# ====================== #
# Compiler warning flags #
# ====================== #
# ----- #
# Intel #
# ----- #
set(C_INTEL_WARNINGS
  "-Wall -Wunused-variable -Wunused-function -Wuninitialized"
)
set(F90_INTEL_WARNINGS
  "-warn all -check all"
)

# --- #
# GNU #
# --- #
set(C_GNU_WARNINGS
  "-Wall -Wextra" #-Wconversion"
)
set(F90_GNU_WARNINGS
  "-Wall -Wextra" #-Wconversion"
)

# ================== #
# Set compiler flags #
# ================== #
if (CMAKE_C_COMPILER_ID MATCHES "Intel")
  # Intel optimization flag
  #add_definitions(-xCore-AVX2)

  # Compiler warnings
  set(CMAKE_C_FLAGS ${C_INTEL_WARNINGS})
  set(CMAKE_FORTRAN_FLAGS ${F90_INTEL_WARNINGS})
elseif (CMAKE_C_COMPILER_ID MATCHES "GNU")
  # Compiler warnings
  set(CMAKE_C_FLAGS ${C_GNU_WARNINGS})
  set(CMAKE_FORTRAN_FLAGS ${F90_GNU_WARNINGS})
endif ()

# ======================================= #
# Compile in single precision
# add_definitions(-DSINGLE_PRECISION)
# ======================================= #

# ======================================= #
# Source directories for this project
set(src_path src)
add_subdirectory (${src_path})
# ======================================= #
