#include <stdio.h>
#include <mpi.h>

#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"

int rsb_setup = 0;
//------------------------------------------------------------------------------
void ax_setup(struct gs_data **gsh, double **weights, struct comm *c, \
                 unsigned int npts, unsigned int nelt, long *glo_num)
{
  *gsh = gs_setup(glo_num, npts, c, 0, gs_auto, 0);

  double *u = malloc(sizeof(double)*npts);
  if (u == NULL) {
    printf("malloc failed in %s:%d", __FILE__, __LINE__);
  }
  int nc = npts/nelt;

  for (unsigned int i = 0; i < nelt; i++) {
    for (int j = 0; j < nc; j++) {
      u[nc*i + j] = 1.;
    }
  }

  gs(u, gs_double, gs_add, 1, *gsh, NULL);

 for (unsigned int i = 0; i < nelt; i++) {
   for (int j = 0; j < nc; j++) {
     printf("u[%d, %d] = %lf\n", i, j, u[nc*i + j]);
   }
 }
  *weights = malloc(sizeof(double)*nelt);
  for (long i = 0; i < nelt; i++) {
    *(*weights + i) = 0;
    for (long j = 0; j < nc; j++) {
      *(*weights + i) += u[nc*i + j];
    }
  }

  free(u);

  rsb_setup = 1;
}
//------------------------------------------------------------------------------
void ax(Vector *v, Vector *u, struct gs_data *gsh, double *weights, long nc) {
  if (rsb_setup == 0) {
    fprintf(stderr, "Need to call ax_setup before this routine.");
    return;
  }

  /* Asserts size of v = size of u
  */
  double *vv = v->vv;
  double *uv = u->vv;
  int size = v->size;

  double *ucv = malloc(sizeof(double)*nc*size);
  if (ucv == NULL) {
    printf("malloc failed in %s:%d", __FILE__, __LINE__);
  }

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < nc; j++) {
      ucv[nc*i + j] = uv[i];
    }
  }

  gs(ucv, gs_double, gs_add, 0, gsh, NULL);

  for (int i = 0; i < size; i++) {
    vv[i] = weights[i]*uv[i];
    for (int j = 0; j < nc; j++) {
      vv[i] -= ucv[nc*i + j];
    }
  }

  free(ucv);
}
//------------------------------------------------------------------------------
