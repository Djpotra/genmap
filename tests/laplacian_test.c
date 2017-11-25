#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"
#include "mpiwrapper.h"
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  // Serial part: TODO: Do in parallel
  long npts, nelt, *glo_num, *header;
  double *weights = NULL;
  int nc;
  unsigned int lpts, lelt, lstart;

  Vector u, v;

  readmap_serial(&header, &glo_num, "nbrhd/nbrhd.map.bin");
  npts = header[NPTS];
  nelt = header[NEL];

  int np, rank;
#ifdef MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np  );
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
  np = 1;
  rank = 0;
  int MPI_COMM_WORLD = 0;
#endif

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

  ax(&v, &u, gsh, weights, lpts/lelt);

  for (unsigned int i = 0; i < lelt; i++) {
    printf("v: %lf\n", v.vv[i]);
  }

  for (unsigned int i = 0; i < lelt; i++) {
    printf("rank = %d, weight[%d] = %lf\n", rank, i, weights[i]);
  }

  if (rank < np/2) {
    for (unsigned int i = 0; i < lelt; i++) {
      u.vv[i] = 0;
    }
  } else {
    for (unsigned int i = 0; i < lelt; i++) {
      u.vv[i] = 1;
    }
  }

  ax(&v, &u, gsh, weights, lpts/lelt);

  for (unsigned int i = 0; i < lelt; i++) {
    printf("v: %lf\n", v.vv[i]);
  }

  comm_free(&c);
  gs_free(gsh);

#ifdef MPI
  MPI_Finalize();
#endif

  delete_vector(&v); delete_vector(&u);

  free(glo_num); free(weights);

  return 0;
}
//------------------------------------------------------------------------------
