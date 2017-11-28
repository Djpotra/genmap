#include "test.h"
#include "linalg.h"
#include "lanczos.h"
#include "mpiwrapper.h"
#include "io.h"

//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  // Serial part: TODO: Do in parallel
  long npts, nelt, *glo_num, *header, *elem_id;
  double *weights;
  int nc;
  int lpts, lelt, lstart;

  Vector init, alpha, beta;

  struct comm c;
  init_genmap(&c, argc, argv);

  // Read the .map file
  readmap(&c, &header, &glo_num, &elem_id, "nbrhd/nbrhd.map.bin");
  npts = header[NPTS];
  nelt = header[NEL];

  // Element distribution after reading the .map file
  int np, rank;
  np = c.np; rank = c.id;

  nc = npts/nelt;
  lelt = nelt/np;
  lstart = rank*lelt;
  if (rank == np - 1) {
    lelt = nelt - lstart;
  }
  lpts = lelt*nc;

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
    printf("beta = [");
    for (int i = 0; i < beta.size; i++) {
      printf("%.17g, ", beta.vv[i]);
    }
    printf("]\n");
    printf("alpha= [");
    for (int i = 0; i < alpha.size; i++) {
      printf("%.17g, ", alpha.vv[i]);
    }
    printf("]\n");

    Vector d, e;
    zeros_vector(&d, iter); zeros_vector(&e, iter);
    copy_vector(&d, &alpha);
    for (int i = 0; i < iter - 1; i++) {
      e.vv[i] = beta.vv[i];
    }
  }

  // Free data structures
  finalize_genmap(&c);
  gs_free(gsh);
  delete_vector(&alpha); delete_vector(&beta);
  delete_vector(&init);

  free(glo_num); free(weights);

  return 0;
}
//------------------------------------------------------------------------------
