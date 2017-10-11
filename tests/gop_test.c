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

  struct comm c;
  comm_init(&c, MPI_COMM_WORLD);

  struct gs_data *gsh;
  long gsid = 1;
  gsh = gs_setup(&gsid, 1, &c, 0, gs_auto, 0);

  double sum = rank;
  gs(&sum, gs_double, gs_add, 0, gsh, NULL);
  if (rank == 0) {
    printf("Sum is : %lf\n", sum);
  }

  double min = rank;
  gs(&min, gs_double, gs_min, 0, gsh, NULL);
  if (rank == 0) {
    printf("Min is : %lf\n", min);
  }

  double max = rank;
  gs(&max, gs_double, gs_max, 0, gsh, NULL);
  if (rank == 0) {
    printf("Max is : %lf\n", max);
  }

  gs_free(gsh);

  comm_free(&c);

  MPI_Finalize();

  return 0;
}
//------------------------------------------------------------------------------
