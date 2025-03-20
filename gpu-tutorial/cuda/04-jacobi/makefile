cudaFlags = -m64 -O3 --use_fast_math

# To use clang in OSX
ifeq ($(shell uname -s),Darwin)
	cudaFlags += -ccbin /usr/bin/clang++ -Xcompiler -stdlib=libstdc++
endif

jacobi: jacobi.cu
	nvcc -o jacobi $(cudaFlags) jacobi.cu

clean:
	rm -f jacobi result.dat
