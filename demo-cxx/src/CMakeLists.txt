include_directories(./okl)
include_directories($ENV{OCCA_DIR}/include)
#message("OCCA_LB: ${occa_lb}")

find_library(occa_lb NAMES occa PATHS $ENV{OCCA_DIR}/lib)

file(GLOB OKL_H ./okl/*.h)
file(GLOB DEMO_H ../include/*.h ../include/*.hxx)

# C++ source files
set(DEMO_SRC
    function_to_macro.cxx
    Platform.cxx
    Solver.cxx
    initialize.cxx
)

add_library(demo SHARED ${DEMO_SRC} ${DEMO_H} ${OKL_H})
target_compile_features(demo PUBLIC cxx_std_17)
target_link_libraries(demo ${occa_lb} m)

include_directories(../include)
add_executable(demo-c++.mpi main.cxx)

target_link_libraries(demo-c++.mpi demo ${occa_lb} m)

install(TARGETS demo-c++.mpi
        RUNTIME DESTINATION bin/
        LIBRARY DESTINATION lib/
        ARCHIVE DESTINATION lib/
)

install(FILES ${DEMO_H} DESTINATION include)
