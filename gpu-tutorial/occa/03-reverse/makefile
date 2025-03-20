OCCA_DIR   = ../../../occa/build
OCCA_INC   = ../../../occa/include
OCCA_LIB   = -locca -lpthread -ldl
OCCA_FLAGS = -I${OCCA_INC} -I${OCCA_DIR}/include -L${OCCA_DIR}/lib -Wl,-rpath=${OCCA_DIR}/lib ${OCCA_LIB}

CXXFLAGS  = -O3 -fPIC -std=c++17

reverse: reverse.cxx
	mpicxx -o reverse reverse.cxx ${CXXFLAGS} ${OCCA_FLAGS}

clean:
	rm -f reverse
