#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"
#include "mpiwrapper.h"
//------------------------------------------------------------------------------
int32 main(int32 argc, char **argv) {
  // Serial part: TODO: Do in parallel
  int32 npts, nelt, *glo_num, *header, *elem_id;
  int32 nc;
  int32 lpts, lelt, lstart;
  int32 np, rank;

  double *weights = NULL;

  Vector u, v;

  struct comm c;

  init_genmap(&c, argc, argv);
  np = c.np; rank = c.id;

  readmap_serial(&header, &glo_num, &elem_id, "nbrhd/nbrhd.map.bin");
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

  ax_init(&gsh, &weights, &c, lpts, lelt, &glo_num[lstart*nc]);

  random_vector(&v, lelt, rank);

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
    printf("v[%d]: %lf\n", i, v.vv[i]);
  }

  gs_free(gsh);
  delete_vector(&v); delete_vector(&u);
  free(glo_num); free(weights);

  finalize_genmap(&c);
  return 0;
}
//------------------------------------------------------------------------------
