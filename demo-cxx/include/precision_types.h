/**
 * \file    precision_types.h
 * \author  akirby
 *
 * \brief   This file defines the precision of Real.
 */

#ifndef PRECISION_TYPES_H
#define PRECISION_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/** double/float data type overload */
#ifndef SINGLE_PRECISION
#  define Real double
#  define Uint unsigned long long int
#  define MPI_DGREAL MPI_DOUBLE
#  define MPI_DGREAL_INT MPI_DOUBLE_INT
#  define RealFormat "%le"
#  define RealString "double"
#  define IntFormat  "%d"
#else
#  define Real float
#  define Uint unsigned long int
#  define MPI_DGREAL MPI_FLOAT
#  define MPI_DGREAL_INT MPI_FLOAT_INT
#  define RealFormat "%f"
#  define RealString "float"
#  define IntFormat  "%d"
#endif

#ifndef UINT
#  define UINT unsigned int
#endif

/* wrap fprintf for dg4est */
#define DGOUT(s_,f_, ...) fprintf((s_),(f_), ##__VA_ARGS__);
#define FUNC_NAME(Variable) printf("Function: %s\n",(#Variable))
#define FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define STOP_ALL(err) MPI_Finalize(); exit((err));

/* display levels */
#define DGLOG_NONE          0
#define DGLOG_MG_OFF        1
#define DGLOG_MESH_OFF      2
#define DGLOG_IGBP_OFF      3
#define DGLOG_STATUS_OFF    4
#define DGLOG_PERF_OFF      5
#define DGLOG_PERFSUM_OFF   6
#define DGLOG_RES_OFF       7
#define DGLOG_RESSUM_OFF    8
#define DGLOG_MINRES_OFF    9
#define DGLOG_ALL          10

#define DGLOG_ROOT(LOG_STATE)  if(ctx->rank == 0 && (ctx->log_info < (LOG_STATE)))

#ifndef SERIALIZE
#  define INNER_ALL @inner
#  define INNER_I   @inner
#  define INNER_J   @inner
#  define INNER_K   @inner
# else
#  define INNER_ALL
#  define INNER_I
#  define INNER_J
#  define INNER_K
#endif

#ifdef __cplusplus
}
#endif
#endif /* PRECISION_TYPES_H */
