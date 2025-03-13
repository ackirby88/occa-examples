program main
  use occa
  use iso_c_binding
  use init_module
  implicit none

  integer(occaUDim_t) :: one = 1
  character(1024)     :: info

  ! Set default OCCA device info
  !info = "{mode: 'Serial'}"
  !info = "{mode: 'OpenMP', schedule: 'compact', chunk: 10}"
  !info = "{mode: 'CUDA'  , device_id: 0}"
  !info = "{mode: 'HIP'   , device_id: 0}"
  info = "{mode: 'OpenCL', platform_id: 1, device_id: 0}"

  ! Print device infos
  call init_setup(info)

  ! Allocate memory on the device
  o_a  = occaDeviceTypedMalloc(device, entries, occaDtypeFloat, C_NULL_ptr, occaDefault)
  o_b  = occaDeviceTypedMalloc(device, entries, occaDtypeFloat, C_NULL_ptr, occaDefault)
  o_val = occaDeviceTypedMalloc(device,    one, occaDtypeDouble, C_NULL_ptr, occaDefault)

  ! We can also allocate memory without a dtype
  ! WARNING: This will disable runtime type checking
  o_ab = occaDeviceMalloc(device, entries*C_float, C_NULL_ptr, occaDefault)

  ! Setup properties that can be passed to the kernel
  props = occaCreateJson()
  call occaJsonObjectSet(props, F_C_str("defines/TILE_SIZE"), occaInt(10))

  !std::string occaCacheDir = std::string(pwd) + "/.occa";
  !call setOccaCacheDir(occaCacheDir);

  ! Compile the kernel at run-time
  addVectors = occaDeviceBuildKernel(device, &
                                     F_C_str("./okl/addVectors.okl"), &
                                     F_C_str("addVectors"), &
                                     props)

  test_okl = occaDeviceBuildKernel(device, &
                                   F_C_str("./okl/test.okl"), &
                                   F_C_str("test_okl"), &
                                   props)

  ! Copy memory to the device
  call occaCopyPtrToMem(o_a, C_loc(a), entries        , 0_occaUDim_t, occaDefault)
  call occaCopyPtrToMem(o_b, C_loc(b), occaAllBytes   , 0_occaUDim_t, occaDefault)

  ! Launch device kernel
  call occaKernelRun(addVectors, occaInt(entries), o_a, o_b, o_ab)
  call occaKernelRun(test_okl, o_val)

  ! Copy result to the host
  call occaCopyMemToPtr(C_loc(ab), o_ab, occaAllBytes, 0_occaUDim_t, occaDefault)
  call occaCopyMemToPtr(C_loc(val), o_val, occaAllBytes, 0_occaUDim_t, occaDefault)

  ! Assert values
  do iu=1,entries
    write(*,'(4xa,i2,a,f3.1)') " ab(", iu, ") = ", ab(iu)
  end do
  do iu=1,entries
    if (abs(ab(iu) - (a(iu) + b(iu))) > 1.0e-8) stop "*** Wrong result ***"
  end do

  ! Display test function value
  write(*,'(4xa,f15.7)') "Value from test_okl: ",val

  ! Free host memory
  deallocate(a, b, ab, val, stat=alloc_err)
  if (alloc_err /= 0) stop "*** Deallocation not successful ***"

  ! Free device memory and OCCA objects
  call occaFree(props)
  call occaFree(addVectors)
  call occaFree(test_okl)
  call occaFree(o_a)
  call occaFree(o_b)
  call occaFree(o_ab)
  call occaFree(o_val)
  call occaFree(device)
end program main
