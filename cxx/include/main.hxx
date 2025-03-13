/**
 * File:   main.hxx
 * Author: akirby
 *
 * Created on February 4, 2024, 6:20 PM
 */

/* system header files */
#include <mpi.h>
#include <occa.h>

/* data header files */
#include "Solver.hxx"

/* header files */
#include "initialize.hxx"

#ifndef MAIN_HXX
#define MAIN_HXX

#ifdef __cplusplus
extern "C" {
#endif

/** Main program function.
 *
 * @param argc      number of command-line arguments
 * @param argv      command-line arguments
 * @return program return value.
 */
int main(int argc,char **argv);

#ifdef __cplusplus
}
#endif
#endif /* MAIN_HXX */
