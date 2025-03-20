module init_module
  use occa
  use iso_c_binding
  implicit none

  integer :: alloc_err, i
  integer(occaUDim_t) :: iu
  integer(occaUDim_t) :: entries = 5
  character(len=1024) :: arg
  type(C_ptr) :: cptr

  real(C_float), allocatable, target :: a(:), b(:), ab(:)
  real(C_double), allocatable, target :: val(:)

  ! OCCA device, kernel, memory and property objects
  type(occaDevice) :: device
  type(occaKernel) :: addVectors,test_okl
  type(occaMemory) :: o_a, o_b, o_ab, o_val
  type(occaJson)   :: props

  contains

  subroutine init_setup(info)
    implicit none

    character(len=1024),intent(inout) :: info

    ! make symbolic link to okl files
    print*,"   SOLVER DIR:",SOLVER_DIR
    call SYMLNK(SOLVER_DIR//"/okl","okl");

    ! Parse command arguments
    i = 1
    do while (i .le. command_argument_count())
      call get_command_argument(i, arg)

      select case (arg)
        case ("-v", "--verbose")
          call occaJsonObjectSet(occaSettings(), "kernel/verbose", occaTrue)
        case ("-d", "--device")
          i = i+1
          call get_command_argument(i, info)
        case ("-h", "--help")
          call print_help()
          stop
        case default
          write(*,'(2a, /)') "Unrecognized command-line option: ", arg
          stop
      end select
      i = i+1
    end do

    ! Allocate host memory
    allocate(a(entries), b(entries), ab(entries), val(1), stat=alloc_err)
    if (alloc_err /= 0) stop "*** Not enough memory ***"

    ! Initialize host arrays
    do iu=1,entries
      a(iu) = real(iu)-1
      b(iu) = 2-real(iu)
    end do
    ab = 0

    ! Print device infos
    call occaPrintModeInfo()

    ! Create OCCA device
    device = occaCreateDeviceFromString(F_C_str(info))

    ! Print device mode
    ! Use an intermediate variable to avoid an ICE with the 2019 Intel compilers
    cptr = occaDeviceMode(device)
    write(*,'(4xa,a)') "occaDeviceMode: ", C_F_str(cptr)
  end subroutine

  subroutine print_help()
    write(*,'(a, /)') "Example adding two vectors"
    write(*,'(a, /)') "command-line options:"
    write(*,'(a)')    "  -v, --verbose     Compile kernels in verbose mode"
    write(*,'(a)')    "  -d, --device      Device properties (default: ""{mode: 'Serial'}"")"
    write(*,'(a)')    "  -h, --help        Print this information and exit"
  end subroutine print_help
end module