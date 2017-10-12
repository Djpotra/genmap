#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"

#include <mpi.h>
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  // Serial part: TODO: Do in parallel
  long npts, nelt, *glo_num;
  double *weights;
  int nc;
  int lpts, lelt, lstart;

  Vector init, alpha, beta;

  // Read the .map file
  readmap(&npts, &nelt, &glo_num, "nbrhd/nbrhd.map");

  // Element distribution after reading the .map file
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

  // Initialize gslib
  struct comm c;
  comm_init(&c, MPI_COMM_WORLD);
  struct gs_data *gsh;
  ax_setup(&gsh, &weights, &c, lpts, lelt, &glo_num[lstart*nc]);

  // Setup variables for lanczos
  int iter = 8;
  zeros_vector (&init , lelt    );
  for (int i = 0; i < lelt; i++) {
    init.vv[i] = (double)lstart + i;
  }
  zeros_vector(&alpha, iter    );
  zeros_vector(&beta , iter - 1);

  // Do lanczos
  lanczos(&alpha, &beta, gsh, weights, nc, &init, iter);
  if (rank == 0) {
    for (int i = 0; i < iter - 1; i++) {
      printf("beta: %lf\n", beta.vv[i]);
    }
  }
//
//  for (int i = 0; i < lelt; i++) {
//    printf("rank = %d, weight[%d] = %lf\n", rank, i, weights[i]);
//  }

  // Free data structures
  comm_free(&c);
  gs_free(gsh);

  MPI_Finalize();

//  delete_vector(&alpha); delete_vector(&beta);
//  delete_vector(&init);

  free(glo_num); free(weights);

  return 0;
}
//------------------------------------------------------------------------------
