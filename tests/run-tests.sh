#!/bin/sh

test_mpi=0
test_valgrind=0

MPI_CMD=
VALGRIND_CMD=
NP_LIST=

if [ $# -gt 0 ]; then
  test_mpi=$1
  if [ $# -gt 1 ]; then
    test_valgrind=$2
  fi
fi

if [ $test_valgrind -eq 1 ]; then
  VALGRIND_CMD="valgrind --leak-check=full"
fi
if [ $test_mpi -eq 1 ]; then
  NP_LIST="2 4"
  MPI_CMD="mpirun -np"
  if [ $test_valgrind -eq 1 ]; then
    VALGRIND_CMD="${VALGRIND_CMD} --suppressions=/usr/share/openmpi/openmpi-valgrind.supp"
  fi
fi

echo "MPI_CMD = ${MPI_CMD}"
echo "VALGRIND_CMD = ${VALGRIND_CMD}"

if [ $test_mpi -eq 1 ]; then
  for np in $NP_LIST; do
    for i in *-test.o; do
      $MPI_CMD $np $VALGRIND_CMD ./$i
    done
  done
else
  for i in *-test.o; do
    $VALGRIND_CMD ./$i
  done
fi
