# genmap

Domain partitioning tool for Nek5000 using the recursive spectral
bisection method.

### Introduction

Currently, the code can perform a RSB iteration in 2D. 

### Build instructions

You can build `libgenmap.a` and the tests using gnu make.

```sh
make
```

Just to build the library use:

```sh
make genmap
```

then to build the tests, do:

```sh
make test
```

### Running tests

To run the tests, just do:

```sh
make test
```

This will run the tests for various algorithms.

### Developer documentation

- tests/ - Contains tests.
