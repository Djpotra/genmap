#!/bin/sh

make -C .. MPI=0
for i in *-test.o; do
  echo $i
done
