#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"
#include "mpiwrapper.h"
//------------------------------------------------------------------------------
int32 main(int32 argc, char **argv) {
  // Serial part: TODO: Do in parallel
  int64 npts, nelt, *glo_num, *header, *elem_id;
  double *weights = NULL;
  int32 nc;
  int32 lpts, lelt, lstart;

  Vector u, v;

  struct comm c;
  init_genmap(&c, argc, argv);
  int32 np, rank;
  np = c.np; rank = c.id;

  readmap_serial(&c, &header, &glo_num, &elem_id, "nbrhd/nbrhd.map.bin");
  npts = header[NPTS];
  nelt = header[NEL];

  nc = npts/nelt;
  lelt = nelt/np;
  lstart = rank*lelt;
  if (rank == np - 1) {
    lelt = nelt - lstart;
  }
  lpts = lelt*nc;

  struct gs_data *gsh;

  ax_setup(&gsh, &weights, &c, lpts, lelt, &glo_num[lstart*nc]);

  parallel_random_vector(&v, lelt, rank);

  ones_vector(&u, lelt);

  ax(&v, &u, gsh, weights, lpts/lelt);

  for (int32 i = 0; i < lelt; i++) {
    printf("v: %lf\n", v.vv[i]);
  }

  for (int32 i = 0; i < lelt; i++) {
    printf("rank = %d, weight[%d] = %lf\n", rank, i, weights[i]);
  }

  if (rank < np/2) {
    for (int32 i = 0; i < lelt; i++) {
      u.vv[i] = 0;
    }
  } else {
    for (int32 i = 0; i < lelt; i++) {
      u.vv[i] = 1;
    }
  }

  ax(&v, &u, gsh, weights, lpts/lelt);

  for (int32 i = 0; i < lelt; i++) {
    printf("v: %lf\n", v.vv[i]);
  }

  finalize_genmap(&c);
  gs_free(gsh);
  delete_vector(&v); delete_vector(&u);
  free(glo_num); free(weights);

  return 0;
}
//------------------------------------------------------------------------------
