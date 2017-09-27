# genmap

Domain partitioning tool for Nek5000 using the recursive spectral
bisection method.

### Introduction

Currently, the code can perform a lanczos iteration to find the
tri-diagonal matrix whoose eigenvalues approximate the eigenvalues of a
given symmetric matirx. Lanczos iteration can be found in
`src/lanczos.c`. There are two implementations (to compare answers)
called `lanczos` and `lanczos2`.

API for calling lanczos iteration looks like below:

```c
void lanczos (Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);
void lanczos2(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);
```

Main data structures used by `lanczos` iterations are `CSRMatrix` and
`Vector`. Here the input arguements are `A` and `init` and `alpha` and
`beta` are the output `Vector`s which contain diagonal and subdiagonal
of the final Hermition tridiagonal matrix produced by lanczos.

Lanczos iteration takes a `CSRMatrix` (say, A) and an initial vector to
be used as the starting vector for the  iteration and then produce two
arrays: diagonal and sub-diagonal entries of the hermitian tri-diagonal
matrix which approximates the eigenvalues of A.

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
./tests/ax_test.o
```

To run the tests for various vector operations,

```sh
make tests
./tests/vector_test.o
```

To run the tests for Lanczos iteration,

```sh
make tests
./tests/lanczos_test.o
```

### Developer documentation

- csr.c/.h - Contains the definition of `CSRMatrix` and implementation
  of `CSRMatrix` and `Vector` product.
- linalg.c/.h - Contains the definition of `Vector` and implementation
  of `Vector` functions.
- random.h - Contains a single variable which keeps track whether
  `srand()` is initialized.
- lanczos.c - Constains the Lanczos iteration implementation.
- tests/ - Contains tests.
