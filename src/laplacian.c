#include <stdio.h>
#include <stdlib.h>

#include "laplacian.h"
#include "linalg.h"
#include "mpiwrapper.h"

int32 rsb_setup = 0;
//------------------------------------------------------------------------------
void ax_setup(struct gs_data **gsh, double **weights, struct comm *c, \
                 int32 npts, int32 nelt, int32 *glo_num)
{
  *gsh = gs_setup(glo_num, npts, c, 0, gs_auto, 0);

  double *u = NULL;
  u = malloc(sizeof(double)*npts);
  if (u == NULL) {
    printf("malloc failed in %s:%d", __FILE__, __LINE__);
  }
  int32 nc = npts/nelt;

  for (int32 i = 0; i < nelt; i++) {
    for (int32 j = 0; j < nc; j++) {
      u[nc*i + j] = 1.;
    }
  }

  gs(u, gs_double, gs_add, 1, *gsh, NULL);

  *weights = malloc(sizeof(double)*nelt);
  for (int64 i = 0; i < nelt; i++) {
    *(*weights + i) = 0;
    for (int64 j = 0; j < nc; j++) {
      *(*weights + i) += u[nc*i + j];
    }
  }

  free(u);

  rsb_setup = 1;
}
//------------------------------------------------------------------------------
void ax(Vector *v, Vector *u, struct gs_data *gsh, double *weights, int64 nc) {
  if (rsb_setup == 0) {
    fprintf(stderr, "Need to call ax_setup before this routine.");
    return;
  }

  /* Asserts size of v = size of u
  */
  double *vv = v->vv;
  double *uv = u->vv;
  int32 size = v->size;

  double *ucv = NULL;
  ucv = malloc(sizeof(double)*nc*size);
  if (ucv == NULL) {
    printf("malloc failed in %s:%d", __FILE__, __LINE__);
  }

  for (int32 i = 0; i < size; i++) {
    for (int32 j = 0; j < nc; j++) {
      ucv[nc*i + j] = uv[i];
    }
  }

  gs(ucv, gs_double, gs_add, 0, gsh, NULL);

  for (int32 i = 0; i < size; i++) {
    vv[i] = weights[i]*uv[i];
    for (int32 j = 0; j < nc; j++) {
      vv[i] -= ucv[nc*i + j];
    }
  }

  free(ucv);
}
//------------------------------------------------------------------------------
