OCCA_DIR   = ../../../occa/build
OCCA_INC   = ../../../occa/include
OCCA_LIB   = -locca -lpthread -ldl
OCCA_FLAGS = -I${OCCA_INC} -I${OCCA_DIR}/include -L${OCCA_DIR}/lib -Wl,-rpath=${OCCA_DIR}/lib ${OCCA_LIB}

CXXFLAGS  = -O3 -fPIC -std=c++17

addVectors: addVectors.cxx
	mpicxx -o addVectors addVectors.cxx ${CXXFLAGS} ${OCCA_FLAGS}

clean:
	rm -f addVectors
