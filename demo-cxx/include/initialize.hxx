/**
 * File:   initialize.hxx
 * Author: akirby
 *
 * Created on February 4, 2024, 7:31 PM
 */

#ifndef INITIALIZE_H
#define INITIALIZE_H

/* system header files */
#include <algorithm>
#include <mpi.h>

/* data header files */
#include "Solver.hxx"

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize MPI library.
 *
 * @param argc
 * @param argv
 * @param comm
 * @param rank
 * @param nrank
 * @return  MPI_init return value.
 */
int initialize_mpi(int argc,char **argv,MPI_Comm comm,int *rank,int *nrank);

/** Initialize CFD solver on Host and Device.
 *
 * @param comm
 * @param rank
 * @param device_mode
 * @param device_platform
 * @return Solver
 */
Solver_t* initialize_solver(MPI_Comm comm,int rank,int device_mode,
                            int device_id=0,int device_platform=0);

#ifdef __cplusplus
}
#endif
#endif /* INITIALIZE_H */