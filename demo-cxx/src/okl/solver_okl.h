/**
 * File:   solver_okl.h
 * Author: akirby
 *
 * Created on February 4, 2024, 6:58 PM
 */

#ifndef SOLVER_OKL_H
#define SOLVER_OKL_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SOURCE 10
#define BUFF_SIZE_OKL 1024

/* element face types */
#define BC_TYPE   1
#define FULL_TYPE 2
#define HANG_TYPE 3

#define PLUS   1
#define MINUS -1

/**< function pointer offsets */
#define _DX  0
#define _DY  1
#define _DZ  2
#define _X   0
#define _Y   1
#define _Z   2

/**< multidimensional argument options*/
#ifdef _2D_
#  define DIM 2
#  define VDIM(x)   ((x) * (x))
#  define SDIM(x)    (x)
#  define ARG3D(x)
#  define E3D(x)
#  define X3D(x)
#  define Real3D(x)
#  define OPDIM(x2d,x3d) (x2d)
#else
#  define DIM 3
#  define VDIM(x)   ((x) * (x) * (x))
#  define SDIM(x)   ((x) * (x))
#  define ARG3D(x)       x,
#  define E3D(x)         x;
#  define X3D(x)         x
#  define Real3D(x) Real x,
#  define OPDIM(x2d,x3d) (x3d)
#endif

/* OKL inlined functions based on simulation dimension (EXAMPLES) */
#ifdef _2D_
#  define ref2phys_okl                                  ref2phys_okl_2D
#  define phys2ref_okl                                  phys2ref_okl_2D
#  define phys2ref_ijac_okl                             phys2ref_ijac_okl_2D
#  define riemann_flux_okl                              riemann_flux_okl_2D
#  define riemann_splitflux_okl                         riemann_splitflux_okl_2D
#  define splitform_surface_okl                         splitform_surface_okl_2D
#  define splitform_volume_okl                          splitform_volume_okl_2D
#  define flux_surface_viscous_okl                      flux_surface_viscous_okl_2D
#  define flux_bc_viscous_okl                           flux_bc_viscous_okl_2D
#  define turbulence_model_okl                          turbulence_model_okl_2D
#  define turbulence_model_ext_okl                      turbulence_model_ext_okl_2D
#  define projection_volume_copy_okl                    projection_volume_copy_okl_2D
#  define projection_volume_tensor_okl                  projection_volume_tensor_okl_2D
#  define projection_volume_collocation_dx_okl          projection_volume_collocation_dx_okl_2D
#  define projection_volume_collocation_dy_okl          projection_volume_collocation_dy_okl_2D
#  define integration_refvol_phiw_tensor_okl            integration_refvol_phiw_tensor_okl_2D
#  define integration_refvol_phiw_collocation_x_okl     integration_refvol_phiw_collocation_x_okl_2D
#  define integration_refvol_phiw_collocation_y_okl     integration_refvol_phiw_collocation_y_okl_2D
#  define projection_xface_tensor_okl                   projection_xface_tensor_okl_2D
#  define projection_yface_tensor_okl                   projection_yface_tensor_okl_2D
#  define projection_xlo_face_copy_okl                  projection_xlo_face_copy_okl_2D
#  define projection_xhi_face_copy_okl                  projection_xhi_face_copy_okl_2D
#  define projection_ylo_face_copy_okl                  projection_ylo_face_copy_okl_2D
#  define projection_yhi_face_copy_okl                  projection_yhi_face_copy_okl_2D
#  define projection_xface_collocation_okl              projection_xface_collocation_okl_2D
#  define projection_yface_collocation_okl              projection_yface_collocation_okl_2D
#  define projection_xface_collocation_dx_okl           projection_xface_collocation_dx_okl_2D
#  define projection_xface_collocation_dy_okl           projection_xface_collocation_dy_okl_2D
#  define projection_yface_collocation_dx_okl           projection_yface_collocation_dx_okl_2D
#  define projection_yface_collocation_dy_okl           projection_yface_collocation_dy_okl_2D
#  define projection_xface_volume_tensor_okl            projection_xface_volume_tensor_okl_2D
#  define projection_yface_volume_tensor_okl            projection_yface_volume_tensor_okl_2D
#  define integration_xface_tensor_okl                  integration_xface_tensor_okl_2D
#  define integration_yface_tensor_okl                  integration_yface_tensor_okl_2D
#  define integration_xface_tensor_affine_okl           integration_xface_tensor_affine_okl_2D
#  define integration_yface_tensor_affine_okl           integration_yface_tensor_affine_okl_2D
#  define integration_xface_collocation_okl             integration_xface_collocation_okl_2D
#  define integration_yface_collocation_okl             integration_yface_collocation_okl_2D
#  define integration_xface_imass_tensor_okl            integration_xface_imass_tensor_okl_2D
#  define integration_yface_imass_tensor_okl            integration_yface_imass_tensor_okl_2D
#  define integration_xface_imass_collocation_okl       integration_xface_imass_collocation_okl_2D
#  define integration_yface_imass_collocation_okl       integration_yface_imass_collocation_okl_2D
#  define integration_xface_collocation_dx_okl          integration_xface_collocation_dx_okl_2D
#  define integration_xface_collocation_dy_okl          integration_xface_collocation_dy_okl_2D
#  define integration_yface_collocation_dx_okl          integration_yface_collocation_dx_okl_2D
#  define integration_yface_collocation_dy_okl          integration_yface_collocation_dy_okl_2D
#  define integration_xface_collocation_affine_dx_okl   integration_xface_collocation_affine_dx_okl_2D
#  define integration_xface_collocation_affine_dy_okl   integration_xface_collocation_affine_dy_okl_2D
#  define integration_yface_collocation_affine_dx_okl   integration_yface_collocation_affine_dx_okl_2D
#  define integration_yface_collocation_affine_dy_okl   integration_yface_collocation_affine_dy_okl_2D

#  define test_okl_x test_okl_x_2D
#  define test_okl_y test_okl_y_2D
#  define test_okl_z test_okl_z_2D

# else /* 3D simulations functions */

#  define ref2phys_okl                                  ref2phys_okl_3D
#  define phys2ref_okl                                  phys2ref_okl_3D
#  define phys2ref_ijac_okl                             phys2ref_ijac_okl_3D
#  define riemann_flux_okl                              riemann_flux_okl_3D
#  define riemann_splitflux_okl                         riemann_splitflux_okl_3D
#  define splitform_surface_okl                         splitform_surface_okl_3D
#  define splitform_volume_okl                          splitform_volume_okl_3D
#  define flux_surface_viscous_okl                      flux_surface_viscous_okl_3D
#  define flux_bc_viscous_okl                           flux_bc_viscous_okl_3D
#  define turbulence_model_okl                          turbulence_model_okl_3D
#  define turbulence_model_ext_okl                      turbulence_model_ext_okl_3D
#  define projection_volume_copy_okl                    projection_volume_copy_okl_3D
#  define projection_volume_tensor_okl                  projection_volume_tensor_okl_3D
#  define projection_volume_collocation_dx_okl          projection_volume_collocation_dx_okl_3D
#  define projection_volume_collocation_dy_okl          projection_volume_collocation_dy_okl_3D
#  define projection_volume_collocation_dz_okl          projection_volume_collocation_dz_okl_3D
#  define integration_refvol_phiw_tensor_okl            integration_refvol_phiw_tensor_okl_3D
#  define integration_refvol_phiw_collocation_x_okl     integration_refvol_phiw_collocation_x_okl_3D
#  define integration_refvol_phiw_collocation_y_okl     integration_refvol_phiw_collocation_y_okl_3D
#  define integration_refvol_phiw_collocation_z_okl     integration_refvol_phiw_collocation_z_okl_3D
#  define projection_xface_tensor_okl                   projection_xface_tensor_okl_3D
#  define projection_yface_tensor_okl                   projection_yface_tensor_okl_3D
#  define projection_zface_tensor_okl                   projection_zface_tensor_okl_3D
#  define projection_xlo_face_copy_okl                  projection_xlo_face_copy_okl_3D
#  define projection_xhi_face_copy_okl                  projection_xhi_face_copy_okl_3D
#  define projection_ylo_face_copy_okl                  projection_ylo_face_copy_okl_3D
#  define projection_yhi_face_copy_okl                  projection_yhi_face_copy_okl_3D
#  define projection_zlo_face_copy_okl                  projection_zlo_face_copy_okl_3D
#  define projection_zhi_face_copy_okl                  projection_zhi_face_copy_okl_3D
#  define projection_xface_collocation_okl              projection_xface_collocation_okl_3D
#  define projection_yface_collocation_okl              projection_yface_collocation_okl_3D
#  define projection_zface_collocation_okl              projection_zface_collocation_okl_3D
#  define projection_xface_collocation_dx_okl           projection_xface_collocation_dx_okl_3D
#  define projection_xface_collocation_dy_okl           projection_xface_collocation_dy_okl_3D
#  define projection_xface_collocation_dz_okl           projection_xface_collocation_dz_okl_3D
#  define projection_yface_collocation_dx_okl           projection_yface_collocation_dx_okl_3D
#  define projection_yface_collocation_dy_okl           projection_yface_collocation_dy_okl_3D
#  define projection_yface_collocation_dz_okl           projection_yface_collocation_dz_okl_3D
#  define projection_zface_collocation_dx_okl           projection_zface_collocation_dx_okl_3D
#  define projection_zface_collocation_dy_okl           projection_zface_collocation_dy_okl_3D
#  define projection_zface_collocation_dz_okl           projection_zface_collocation_dz_okl_3D
#  define projection_xface_volume_tensor_okl            projection_xface_volume_tensor_okl_3D
#  define projection_yface_volume_tensor_okl            projection_yface_volume_tensor_okl_3D
#  define projection_zface_volume_tensor_okl            projection_zface_volume_tensor_okl_3D
#  define integration_xface_tensor_okl                  integration_xface_tensor_okl_3D
#  define integration_yface_tensor_okl                  integration_yface_tensor_okl_3D
#  define integration_zface_tensor_okl                  integration_zface_tensor_okl_3D
#  define integration_xface_tensor_affine_okl           integration_xface_tensor_affine_okl_3D
#  define integration_yface_tensor_affine_okl           integration_yface_tensor_affine_okl_3D
#  define integration_zface_tensor_affine_okl           integration_zface_tensor_affine_okl_3D
#  define integration_xface_collocation_okl             integration_xface_collocation_okl_3D
#  define integration_yface_collocation_okl             integration_yface_collocation_okl_3D
#  define integration_zface_collocation_okl             integration_zface_collocation_okl_3D
#  define integration_xface_imass_tensor_okl            integration_xface_imass_tensor_okl_3D
#  define integration_yface_imass_tensor_okl            integration_yface_imass_tensor_okl_3D
#  define integration_zface_imass_tensor_okl            integration_zface_imass_tensor_okl_3D
#  define integration_xface_imass_collocation_okl       integration_xface_imass_collocation_okl_3D
#  define integration_yface_imass_collocation_okl       integration_yface_imass_collocation_okl_3D
#  define integration_zface_imass_collocation_okl       integration_zface_imass_collocation_okl_3D
#  define integration_xface_collocation_dx_okl          integration_xface_collocation_dx_okl_3D
#  define integration_xface_collocation_dy_okl          integration_xface_collocation_dy_okl_3D
#  define integration_xface_collocation_dz_okl          integration_xface_collocation_dz_okl_3D
#  define integration_yface_collocation_dx_okl          integration_yface_collocation_dx_okl_3D
#  define integration_yface_collocation_dy_okl          integration_yface_collocation_dy_okl_3D
#  define integration_yface_collocation_dz_okl          integration_yface_collocation_dz_okl_3D
#  define integration_zface_collocation_dx_okl          integration_zface_collocation_dx_okl_3D
#  define integration_zface_collocation_dy_okl          integration_zface_collocation_dy_okl_3D
#  define integration_zface_collocation_dz_okl          integration_zface_collocation_dz_okl_3D
#  define integration_xface_collocation_affine_dx_okl   integration_xface_collocation_affine_dx_okl_3D
#  define integration_xface_collocation_affine_dy_okl   integration_xface_collocation_affine_dy_okl_3D
#  define integration_xface_collocation_affine_dz_okl   integration_xface_collocation_affine_dz_okl_3D
#  define integration_yface_collocation_affine_dx_okl   integration_yface_collocation_affine_dx_okl_3D
#  define integration_yface_collocation_affine_dy_okl   integration_yface_collocation_affine_dy_okl_3D
#  define integration_yface_collocation_affine_dz_okl   integration_yface_collocation_affine_dz_okl_3D
#  define integration_zface_collocation_affine_dx_okl   integration_zface_collocation_affine_dx_okl_3D
#  define integration_zface_collocation_affine_dy_okl   integration_zface_collocation_affine_dy_okl_3D
#  define integration_zface_collocation_affine_dz_okl   integration_zface_collocation_affine_dz_okl_3D

#  define test_okl_x test_okl_x_3D
#  define test_okl_y test_okl_y_3D
#  define test_okl_z test_okl_z_3D
#endif

/* unwrapped function arrays (EXAMPLES) */
// projection_volume_derivative: set in Solver.cxx: pointers_volume_derivative_projection_device
#define projection_volume_derivative_okl_0(D) projection_volume_derivative_X_okl(D)
#define projection_volume_derivative_okl_1(D) projection_volume_derivative_Y_okl(D)
#define projection_volume_derivative_okl_2(D) projection_volume_derivative_Z_okl(D)
#define projection_volume_derivative_okl(N,D) projection_volume_derivative_okl_##N(D) // unwraps N={0,1,2} "token-pasting"

// integration_volume: set in Solver.cxx: pointers_volume_integration_device
#define integration_volume_okl_0 = integration_volume_X_okl;
#define integration_volume_okl_1 = integration_volume_Y_okl;
#define integration_volume_okl_2 = integration_volume_Z_okl;
#define integration_volume_okl(N)  integration_volume_okl_##N // unwraps N={0,1,2} "token-pasting"

// projection functions: set in Solver.cxx
#define projection_surface_okl(N)                     projection_surface_okl_##N                        // unwraps N={FaceID} "token-pasting"
#define projection_surface_tensor_okl(N)              projection_surface_tensor_okl_##N                 // unwraps N={FaceID}
#define projection_surface_derivative_okl(N,D)        projection_surface_derivative_okl_##N_##D         // unwraps N={FaceID}, D={_DX,_DY,_DZ}
#define projection_surface_derivative_tensor_okl(N,D) projection_surface_derivative_tensor_okl_##N_##D  // unwraps N={FaceID}, D={_DX,_DY,_DZ}
#define projection_surface_volume_tensor_okl(N)       projection_surface_volume_tensor_okl_##N          // unwraps N={FaceID}

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
} o_flow_consts_t;

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

    char heading_dir[BUFF_SIZE_OKL]; /**< reference heading direction: x, y, or z */
    char altitude_dir[BUFF_SIZE_OKL];/**< reference vertical direction:  x, y, or z */
    int forces_plane;       /**< force span-wise direction:
                             *  * 1: xy-plane
                             *  * 2: xz-plane
                             *  * 3: yz-plane */
    o_flow_consts_t fc;     /**< derived flow constants */
} o_physics_t;

typedef struct {
    int pdegree_min;        /**< lowest alloable p-degree in simulation */
    int pdegree_max;        /**< highest alloable p-degree in simulation */
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
} o_numerics_t;

#ifdef __cplusplus
}
#endif
#endif /* SOLVER_OKL_H */
