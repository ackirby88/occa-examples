cudaFlags = -m64 -O3 --use_fast_math -Wno-deprecated-gpu-targets

# To use clang in OSX
ifeq ($(shell uname -s),Darwin)
	cudaFlags += -ccbin /usr/bin/clang++ -Xcompiler -stdlib=libstdc++
endif

simple: simple.cu
	nvcc -o simple $(cudaFlags) simple.cu

clean:
	rm -f simple 
