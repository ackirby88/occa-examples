include_directories(./okl)
include_directories($ENV{OCCA_DIR}/include)

find_library(occa_lb NAMES occa PATHS $ENV{OCCA_DIR}/lib)
message("OCCA: ${occa_lb}")

file(GLOB OKL_H ./okl/*.h)
file(GLOB DEMO_H ../include/*.h ../include/*.hxx)

# Fortran source files
set(DEMO_SRC
    init_mod.F90
)

add_library(demo SHARED ${DEMO_SRC} ${DEMO_H} ${OKL_H})
#target_compile_features(demo PUBLIC cxx_std_17)
target_link_libraries(demo ${occa_lb} m)

#include_directories(../include)
add_executable(demo-f90.mpi main.F90)

target_link_libraries(demo-f90.mpi demo ${occa_lb} m)

install(TARGETS demo-f90.mpi
        RUNTIME DESTINATION bin/
        LIBRARY DESTINATION lib/
        ARCHIVE DESTINATION lib/
)

install(FILES ${DEMO_H} DESTINATION include)
