# genmap

Domain partitioning tool for Nek5000 using the recursive spectral bisection
method.

### Introduction

Currently, the code can perform a lanczos iteration to find the tri-diagonal
matrix whoose eigenvalues approximate the eigenvalues of A. Lanczos iteration
can be found in `src/lanczos.c`.

Main data structures used by `genmap` are `CSRMatrix` and `Vector`. Lanczos
iteration takes a `CSRMatrix` (say, A) and an initial vector to be used as the
starting vector for the lanczos iteration and then produce two arrays: diagonal
and sub-diagonal entries of the hermitian tri-diagonal matrix which approximates 
the eigenvalues of A. 0th entries of both of this arrays should be ignored.

Below is the directory structure of the current genmap directory.

```
.
├── inc
│   ├── csr.h
│   ├── linalg.h
│   ├── random.h
│   └── test.h
├── Makefile
├── README.md
├── src
│   ├── csr.c
│   ├── lanczos.c
│   └── linalg.c
└── tests
    ├── ax_test.c
    ├── lanczos_test.c
    ├── test.c
    └── vector_test.c

```

### Build instructions

You can build `libgenmap.so` and the tests using gnu make.

```sh
make
```

Just to build the library use:

```sh
make libgenmap.so
```

then to build the tests, do:
```sh
make tests
```

### Running tests

Currently there are tests for matrix (CSR format) vector multiplication and
vector operators. Tests fot lanczos iteration just call the lanczos method,
it does not test anything. The tests for lanczos will be added later.

To run the tests for matrix-vector multiplication,
```sh
make tests
./tests/ax_tests.o
```
To run the tests for various vector operations,
```sh
make tests
./tests/vector_tests.o
```
