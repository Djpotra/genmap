#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"

//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  // Serial part: TODO: Do in parallel
  long npts, nelt, *glo_num, *weights;
  long lpts, lelt, lstart;

  Vector u, v;

  readmap(&npts, &nelt, &glo_num, "nbrhd.map");

  int np, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np  );
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  lpts = npts/np;
  lelt = nelt/np;
  lstart = rank*lpts;

  struct comm c;
  comm_init(&c, MPI_COMM_WORLD);
  struct gs_data *gsh;

  ax_setup(gsh, &weights, &c, lpts, lelt, &glo_num[lstart]);

  for (int i = 0; i < lelt; i++) {
    printf("rank = %d, lelt = %d, weight = %d\n", rank, i, weights[i]);
  }
//  zero_vector(

  ax(&v, &u, gsh);

  comm_free(&c);

  MPI_Finalize();
  return 0;
}
//------------------------------------------------------------------------------
