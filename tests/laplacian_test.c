#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"

//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  // Serial part: TODO: Do in parallel
  long npts, nelt, *glo_num;
  double *weights;
  int nc;
  unsigned int lpts, lelt, lstart;

  Vector u, v;

  readmap(&npts, &nelt, &glo_num, "nbrhd/nbrhd.map");

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

  ax_setup(&gsh, &weights, &c, lpts, lelt, &glo_num[lstart*nc]);

  parallel_random_vector(&v, lelt, rank);

  ones_vector(&u, lelt);
//  for (int i = 0; i < lelt; i++) {
//    u.vv[i] = (double)(i+1);
//    printf("u[%d]=%lf\n",i,u.vv[i]);
//  }

//  if (rank == 0) {
//    for (int i = 0; i < npts; i++) {
//      printf("glo_num[%d] = %ld\n", i, glo_num[i]);
//    }
//  }

  ax(&v, &u, gsh, weights, lpts/lelt);

//  for (int i = 0; i < lelt; i++) {
//    printf("v after: %lf\n", v.vv[i]);
//  }

//  for (int i = 0; i < lelt; i++) {
//    printf("rank = %d, weight[%d] = %lf\n", rank, i, weights[i]);
//  }

  comm_free(&c);
  gs_free(gsh);

  MPI_Finalize();

//  delete_vector(&v); delete_vector(&u);

  free(glo_num); free(weights);

  return 0;
}
//------------------------------------------------------------------------------
