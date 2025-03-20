cudaFlags = -m64 -O3 --use_fast_math

# To use clang in OSX
ifeq ($(shell uname -s),Darwin)
	cudaFlags += -ccbin /usr/bin/clang++ -Xcompiler -stdlib=libstdc++
endif

reverse: reverse.cu
	nvcc -o reverse $(cudaFlags) reverse.cu

clean:
	rm -f reverse 
