#!/bin/sh

test_mpi=0
test_valgrind=0

MPI_CMD=
VALGRIND_CMD=

if [ $# -gt 0 ]; then
  test_mpi=$1
  if [ $# -gt 1 ]; then
    test_valgrind=$2
  fi
fi

if [ $test_valgrind -eq 1 ]; then
  VALGRIND_CMD="valgrind"
fi
if [ $test_mpi -eq 1 ]; then
  MPI_CMD="mpirun -np 4"
  if [ $test_valgrind -eq 1 ]; then
    VALGRIND_CMD="${VALGRIND_CMD} --suppressions=/usr/share/openmpi/openmpi-valgrind.supp"
  fi
fi

echo "MPI_CMD = ${MPI_CMD}"
echo "VALGRIND_CMD = ${VALGRIND_CMD}"

for i in *-test.o; do
  $MPI_CMD $VALGRIND_CMD ./$i
done
