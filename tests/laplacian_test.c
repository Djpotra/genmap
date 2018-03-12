#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"
#include "mpiwrapper.h"
//------------------------------------------------------------------------------
int32 main(int32 argc, char **argv) {
  struct comm c;
  init_genmap(&c, argc, argv);
  int32 np = c.np; int32 rank = c.id;

  struct element *elements; struct header mapheader;
  readmap(&c, &elements, &mapheader, "nbrhd/nbrhd.map.bin");

  int32 nc = mapheader.nc;
  int32 lelt = mapheader.lelt;
  int32 lpts = nc * lelt;
  int32 *glo_num; glo_num = malloc(sizeof(int32) * lpts);
  for(int32 i = 0; i < lpts; i++) {
    glo_num[i] = elements[i].globalId;
  }

  double *weights = NULL;
  struct gs_data *gsh;
  ax_init(&gsh, &weights, &c, lpts, lelt, glo_num);

  Vector u, v;
  random_vector(&v, lelt, rank); ones_vector(&u, lelt);

  ax(&v, &u, gsh, weights, lpts / lelt);

  for(int32 i = 0; i < lelt; i++) {
    printf("v: %lf\n", v.vv[i]);
  }

  for(int32 i = 0; i < lelt; i++) {
    printf("rank = %d, weight[%d] = %lf\n", rank, i, weights[i]);
  }

  if(rank < np / 2) {
    for(int32 i = 0; i < lelt; i++) {
      u.vv[i] = 0;
    }
  } else {
    for(int32 i = 0; i < lelt; i++) {
      u.vv[i] = 1;
    }
  }

  ax(&v, &u, gsh, weights, lpts / lelt);

  for(int32 i = 0; i < lelt; i++) {
    printf("v[%d]: %lf\n", i, v.vv[i]);
  }

  gs_free(gsh);
  delete_vector(&v); delete_vector(&u);
  free(glo_num); free(weights);

  finalize_genmap(&c);
  return 0;
}
//------------------------------------------------------------------------------
