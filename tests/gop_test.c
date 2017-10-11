#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"

#include <mpi.h>
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  int np, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np  );
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double sum = rank;
  gop(&sum, gs_double, gs_add, 0);
  if (rank == 0) {
    printf("Sum is : %lf\n", sum);
  }

  double min = rank;
  gop(&min, gs_double, gs_min, 0);
  if (rank == 0) {
    printf("Min is : %lf\n", min);
  }

  double max = rank;
  gop(&max, gs_double, gs_max, 0);
  if (rank == 0) {
    printf("Max is : %lf\n", max);
  }

  MPI_Finalize();

  return 0;
}
//------------------------------------------------------------------------------