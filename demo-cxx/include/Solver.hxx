/**
 * \file    Solver.hxx
 * \author  akirby
 * Created on February 4, 2024, 6:34 PM
 */

#ifndef SOLVER_HXX
#define SOLVER_HXX

/* system header files */
#include <cmath>
#include <vector>

/* data header files */
#include "precision_types.h"
#include "Memory.hxx"
#include "Platform.hxx"
#include "function_to_macro.hxx"

/* okl data header files */
#include "solver_okl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BUFF_SIZE 1024      /**< @brief string buffer size */
#define MAX_SOURCE  10      /**< @brief max number of source types */
#define BC_NPROPS    7      /**< @brief number of boundary condition properties */

/* =============== */
/* DATA STRUCTURES */
/* =============== */
typedef struct {
    Real gm1;               /**< \gamma - 1.0 */
    Real gOgm1;             /**< \gamma / \gm1 */
    Real oneOgm1;           /**< 1.0 / \gm1 */
    Real oneOgamma;         /**< 1.0 / \gamma */
    Real mgOprandtl_num;    /**< -\gamma / Pr */
    Real mgOprandtl_turb;   /**< -\gamma / Pr_t */
    Real T0;                /**< ref. temperature */
    Real RT0;               /**< R * \T0 */
    Real oneORT0;           /**< 1.0 / \RT0 */
    Real RC;                /**< R * Sutherland's constant fo gas (air) */
    Real RT0pRC;            /**< \RT0 + \RC */
    Real mu0;               /**< ref. viscosity in (Pa·s) at ref. temperature T0 */
    Real mu0RT0pRC;         /**< \mu0 / \RT0 + \RC */
    Real cs_cp;             /**< \c_s * \c_p */
    Real ca;                /**< cos(RAD(alpha)) */
    Real sa;                /**< sin(RAD(alpha)) */
    Real cca;               /**< cos(RAD(beta)) */
    Real ssa;               /**< sin(RAD(beta)) */
    Real nd_term;           /**< inviscid non-dimensionalization term */
    Real vnd_term;          /**< viscous non-dimensionalization term */
    Real oneOrho0;          /**< 1.0 / \rho0 */
    Real rho0;              /**< ref. density */
    Real rhou0;             /**< ref. x-momentum */
    Real rhov0;             /**< ref. y-momentum */
    Real rhow0;             /**< ref. z-momentum */
    Real rhoe0;             /**< ref. energy-density */
    Real u0;                /**< ref. x-velocity */
    Real v0;                /**< ref. y-velocity */
    Real w0;                /**< ref. z-velocity */
    Real C0;                /**< ref. speed of sound non-dimensional */
    Real C0_dim;            /**< ref. speed of sound dimensional */
    Real V0;                /**< \mach * \C0 */
    Real time_c;            /**< characteristic time */
    Real TG_length;         /**< ref. Taylor Green length: assumed L=1.0 for [-pi*L,pi*L] domain */
    Real oneOTG_length;     /**< 1.0 / Taylor_Green_length */
    Real gravity0;          /**< ref. gravity force non-dimensional */
    Real gravity0_dim;      /**< ref. gravity force dimensional */
    Real gravity[3];        /**< direction vector of the gravity force */
    Real gravity_dim[3];    /**< direction vector of the gravity force */
    Real coriolis_omega0;   /**< ref. coriolis omega non-dimensional */
    Real coriolis_omega0_dim; /**< ref. coriolis omega dimensional */
    Real coriolis_vector[3]; /**< direction vector of the Coriolis force */
    Real coriolis_vector0[3]; /**< Coriolis force unrotated */
    Real rotation_matrix[3][3]; /**< rotation matrix */
    Real grid_align_matrix[3][3]; /**< grid align matrix */
    Real rotate_align_matrix[3][3]; /**< rotation grid align matrix */
} flow_consts_t;

typedef struct {
    Real gamma;             /**< ratio of specific heats */
    Real density0;          /**< initial fluid density: nondimensional */
    Real pressure0;         /**< initial fluid pressure: nondimensional */
    Real mach0;             /**< initial fluid Mach number */
    Real mach_ref;          /**< reference Mach number for Reynolds number calculation */
    Real reynolds_num;      /**< Reynolds number */
    Real reynolds_length;   /**< Reynolds length: grid units */
    Real prandtl_num;       /**< Prandtl number */
    Real alpha;             /**< angle of incidence (angle of attack): degrees */
    Real beta;              /**< yaw angle: degrees */
    Real ref_area;          /**< reference area for force calculation in grid units */
    Real ref_length;        /**< reference length for force calculation in grid units */
    Real compass_angle;     /**< reference compass angle of \heading_dir: degrees */
    Real degrees_latitude;  /**< reference latitude for coriolis force: degrees */
    Real physics_clock;     /**< physics time for initialization */

    int isource[MAX_SOURCE];/**< source body forces */
    int nsource;            /**< number of source body forces */
    int source_ABL_forcing; /**< atmospheric boundary layer forcing source flag (TODO) */
    int source_gravity;     /**< gravity source flag */
    int source_coriolis;    /**< coriolis source flag */

    char heading_dir[BUFF_SIZE]; /**< reference heading direction: x, y, or z */
    char altitude_dir[BUFF_SIZE];/**< reference vertical direction:  x, y, or z */
    int forces_plane;       /**< force span-wise direction:
                             *  * 1: xy-plane
                             *  * 2: xz-plane
                             *  * 3: yz-plane */
    flow_consts_t fc;     /**< derived flow constants */
} physics_t;

typedef struct {
    int pdegree_min;        /**< lowest allowable p-degree in simulation */
    int pdegree_max;        /**< highest allowable p-degree in simulation */
    int nfields;            /**< number of unknowns stored at a solution point */
    int neqn;               /**< number of equations to solve:
                             *  * [2D]:
                             *  *  * zero-equation turbulence model: 4
                             *  *  *  one-equation turbulence model: 5
                             *  * [3D]:
                             *  *  * zero-equation turbulence model: 5
                             *  *  *  one-equation turbulence model: 6  */
    int nfields_project;    /**< number of fields to project solution */
    int iturb;              /**< equation set and turbulence model:
                             *  * 0: Inviscid Flow Equations (Euler)
                             *  * 1: Laminar Compressible Navier-Stokes (no turb. model)
                             *  * 2: LES-Local Wall Adapting Local Eddy-Viscosity(WALE): 0-eqn model
                             *  * 3: LES-Local Constant Smagorinsky: 0-eqn model
                             *  * 4: LES-Wall Adapting Local Eddy-Viscosity(WALE): 0-eqn model (eddy viscosity model)
                             *  * 5: LES-Constant Smagorinsky: 0-eqn model (eddy viscosity model)
                             *  * 6: LES-Dynamic Smagorinsky:  0-eqn model (eddy viscosity model)
                             *  * 7: LES-SGS Kinetic Energy transport: 1-eqn model (TODO) */
    int iturb_smag_inline;  /**< inline LES model computation without eddy viscosity storage */
    int viscous;            /**< viscous flag:
                             *  * 0: iturb = 0
                             *  * 1: iturb > 1 */
    int eddy_viscosity;     /**< eddy viscosity model flag:
                             *  * 0: iturb = {0,1,5,6}
                             *  * 1: iturb = {2-4} */
    int artificial_viscosity; /**< artificial viscosity flag */
    int split_form_type;    /**< split form equation discretization:
                             *  * 0: Standard DG Weak Form
                             *  * 1: DG Standard Form (no splitting)
                             *  * 2: Pirozzoli Split Form
                             *  * 3: Kennedy & Gruber Split Form */
    int initial_condition;  /**< initial condition:
                             *  * 0: free stream
                             *  * 1: isentropic vortex
                             *  * 2: ringleb flow
                             *  * 3: Taylor-Green vortex */
    int boundary_ic;        /**< Boundary initial condition:
                             *  * 0: free stream
                             *  * 1: isentropic vortex
                             *  * 2: ringleb flow
                             *  * 3: Taylor-Green vortex */
    int boundary_conditions[6]; /**< boundary conditions:
                                 *  * 0: characteristic
                                 *  * 1: slip wall
                                 *  * 2: nonslip wall */

    int basis_type;         /**< basis polynomial type:
                             *   * 0: Lagrange interpolating polynomials (nodal)
                             *   * 1: Legendre polynomials (modal)
                             *   * 2: Bernstein polynomails (modal)
                             */
    int nodal_basis;        /**< nodal basis flag */
    int qdegree;            /**< mapping basis degree */
    int quadrature_type;    /**< quadrature type:
                             *   * 0: Legendre-Gauss
                             *   * 1: Lobatto-Gauss
                             */
    int collocation;        /**< solution-integration points collocation flag:
                             *   * 0: OFF (use tensor-product numerics)
                             *   * 1: ON  (use collocation numerics)
                             */
    int overintegration_npts; /**< number of points to over-integrate:
                               *  * Ignored if \collocation = 1
                               */
    int filter_type;        /**< filter solution flag:
                             *  * 0: OFF
                             *  * 1: ON */
    int grid_filter_pdegree;/**< grid filter solution to this p-degree */
    int test_filter_pdegree;/**< test filter solution for dynamic LES models */
    int filter_bc[6][2];    /**< filter boundary cell[side][2]:
                             *  * [0]: filter flag {0: OFF, 1: ON}
                             *  * [1]: filter p-degree */
    int riemann_type;       /**< Riemann solver type:
                             *  * 0: Lax-Friedrichs
                             *  * 1: Roe (TODO)
                             *  * 2: Roe with Entropy fix (TODO)
                             *  * 3: Roe-L2 (TODO)
                             *  * 4: Roe-Low Mach (TODO) */
    int diff_flux_type;     /**< Numerical diffusive flux method:
                             *  * 0: SIP
                             *  * 1: BR2 */
    int unstructured_grid;  /**< Unstructured grid flag:
                             *  * 0: OFF
                             *  * 1: ON
                             */
    int acoustics_lib;      /**< Acoustics analysis library
                             *  * 0: OFF
                             *  * 1: ON
                             */
    Real av_scale_factor;        /**< Artificial viscosity scale factor */
    Real av_activation_limit;    /**< Artificial viscosity activation limit */
    Real riemann_diss_scaler;    /**< Riemann flux dissipation scale factor */
    Real viscous_penalty_scaler; /**< Viscous Penalty scaler factor */
    Real prandtl_turb;           /**< Prandtl number turbulent  */
    Real smagorinsky_const;      /**< LES Constant Smagorinsky constant: 0.12-0.19 */
    Real smagorinsky_scaler;     /**< LES Constant Smagorinsky scale factor: 1.0 */
    Real smagorinsky_ratio_max;  /**< LES Dynamic Smagorinsky ratio limiter max (vs. const. smag) */
    Real smagorinsky_ratio_min;  /**< LES Dynamic Smagorinsky ratio limiter min (vs. const. smag) */
    Real wale_const;             /**< LES WALE constant: 0.325 */
} numerics_t;

class bc_t {
  public:
    char patch_name[BUFF_SIZE]; /**< Physical patch name tag */
    int  patch_dim;             /**< Physical patch dimension */
    int  bc_type;               /**< BC type ID associated with this patch */
    int  bc_filter;             /**< BC filtering flag: [0] ON/OFF */
    char bc_name[BUFF_SIZE];    /**< BC type name corresponding to bc_type */
    Real bc_props[BC_NPROPS];   /**< BC properties associated with this patch */

    /* constructors */
    bc_t()=default;
   ~bc_t()=default;
};

/* =================================== *
 * Solver Data Type:                   *
 *   This data serves as the primary   *
 *   data structure for a solver data. *
 *   It contains all the data types    *
 *   defined above.                    *
 * =================================== */
class solver_data_t {
  public:
    Platform& platform; /**< computing platform */
    char device;        /**< device flag */
    int log_info;       /**< logging threshold flag:
                         *  *  0: log everything
                         *  *  1: prefix file and line number
                         *  *  2: information on the internal state
                         *  *  3: information on conditions, decisions
                         *  *  4: main information a function is doing
                         *  *  5: important consistency/performance information
                         *  *  6: few lines for a major API function
                         *  *  7: logs a few lines max per program
                         *  *  8: logs errors only
                         *  *  9: never logs anything */
    Real memory;        /**< memory allocated in structure */

    physics_t physics;     /**< fluid physics data */
    numerics_t numerics;   /**< numerics data */
    amc::memory<bc_t> bcs; /**< boundary_conditions */

    /* constructors */
    solver_data_t(Platform &platform_,char device_) :
        platform(platform_),
        device(device_) {};

    solver_data_t() :
        platform(*static_cast<Platform*>(NULL)),
        device(0) {};

   ~solver_data_t()=default;
};

/* ================================= *
 * Solver Type:                      *
 *   This data serves as the primary *
 *   data structure for a Solver.    *
 * ================================= */
class Solver_t {
  public:
    Platform& platform;
    char singlePrecWarning;
    char verbose;

    /* solver data */
    solver_data_t solver_data;

    /* device data */
    amc::memory<Real> scratch;
    amc::deviceMemory<Real> o_scratch;
    amc::deviceMemory<o_physics_t> o_physics;
    amc::deviceMemory<o_numerics_t> o_numerics;

    /* device kernels */
    amc::kernel_t norm2;
    amc::kernel_t axpby;
    amc::kernel_t vec_scale;
    amc::kernel_t inner_product;
  //amc::kernel_t single2double;
  //amc::kernel_t double2single;

    amc::kernel_t test_okl;
    amc::kernel_t test_okl_func;
    amc::kernel_t test_okl_struct;
    amc::kernel_t addVectors;
    amc::kernel_t jacobi;

    /* constructors */
    Solver_t(Platform &platform_,MPI_Comm comm,int rank,char verbose_) :
        platform(platform_),
        CPU_SOLVER(0),
        ROOT{(rank==0)},
        rank{rank},
        comm{comm},
        singlePrecWarning(0),
        verbose(verbose_),
        solver_data(platform_,1)
    {
        /* set device property after reading inputs */
        platform.props["defines/NVAR"] = (DIM==2) ? 4:5;
        platform.props["defines/p_Nblock"] = 32;
        platform.props["defines/p_blockSize"] = 256;

        scratch.malloc(platform.props["defines/p_blockSize"]);

        /* create symbolic link for including headers in OKL files */
        int errval = symlink(SOLVER_DIR "/okl","okl");
        if(errval && errno != EEXIST){
            printf("[ERROR] Issue with creating symbolic link for okl directory.\n"
                   "        Error number: %d\n",errno);
        }
    };

    Solver_t(MPI_Comm comm,int rank,char verbose_) :
        platform(*static_cast<Platform*>(NULL)),
        CPU_SOLVER{1},
        ROOT{(rank==0)},
        rank{rank},
        comm{comm},
        singlePrecWarning(0),
        verbose(verbose_),
        solver_data() {};

   ~Solver_t()=default;

    /* class functions */
    char getDeviceCompute(){return !CPU_SOLVER;}
    void setupDeviceAndKernels();
    void init(); // initialize solver on CPU

  private:
    /* data members */
    const int FBASE = 1;
    char CPU_SOLVER = 0;
    char ROOT = 0;

    MPI_Comm comm;
    int rank;

    /* class method functions */
    void initialize_solver_functions_device(int test_func_type);
    void initialize_solver_functions_macros();
    Real resNorm2(std::size_t length,
                  amc::deviceMemory<Real> vec,
                  char normalize);
};

/* global variable declaration */
extern Solver_t *Solver;
extern Platform *computeDevice;

#ifdef __cplusplus
}
#endif
#endif /* SOLVER_HXX */
