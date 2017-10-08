#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"

//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  // Serial part: TODO: Do in parallel
  long npts, nelt, *glo_num;
  int *weights, nc;
  unsigned int lpts, lelt, lstart;

  Vector u, v;

  readmap(&npts, &nelt, &glo_num, "nbrhd.map");

  int np, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np  );
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  nc = npts/nelt;
  lelt = nelt/np;
  lstart = rank*lelt;
  if (rank == np - 1) {
    lelt = nelt - lstart;
  }
  lpts = lelt*nc;

  struct comm c;
  comm_init(&c, MPI_COMM_WORLD);
  struct gs_data *gsh;

  ax_setup(&gsh, &weights, &c, lpts, lelt, &glo_num[lstart]);

//  for (int i = 0; i < lelt; i++) {
//    printf("rank = %d, lelt = %d, weight = %d\n", rank, i, weights[i]);
//  }

  random_vector(&v, lelt); ones_vector(&u, lelt);

//  for (int i = 0; i < lelt; i++) {
//    printf("v before: %lf\n", v.vv[i]);
//  }

  ax(&v, &u, gsh, weights, lpts/lelt);

  for (int i = 0; i < lelt; i++) {
    printf("v after: %lf\n", v.vv[i]);
  }

  comm_free(&c);

  MPI_Finalize();
  return 0;
}
//------------------------------------------------------------------------------
