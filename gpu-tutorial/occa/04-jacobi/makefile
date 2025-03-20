OCCA_DIR   = ../../../occa/build
OCCA_INC   = ../../../occa/include
OCCA_LIB   = -locca
OCCA_FLAGS = -I${OCCA_INC} -I${OCCA_DIR}/include -L${OCCA_DIR}/lib -Wl,-rpath=${OCCA_DIR}/lib ${OCCA_LIB}

CXXFLAGS  = -O3 -fPIC -std=c++17

jacobi: jacobi.cxx
	mpicxx -o jacobi jacobi.cxx ${CXXFLAGS} ${OCCA_FLAGS}

clean:
	rm -f jacobi result.dat
