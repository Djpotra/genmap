#include <stdio.h>
#include <mpi.h>

#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"

int rsb_setup = 0;
//------------------------------------------------------------------------------
void ax_setup(struct gs_data **gsh, long **weights, struct comm *c, \
                            long npts, long nelt, long *glo_num)
{
  *gsh = gs_setup(glo_num, npts, c, 0, gs_crystal_router, 0);

  long *u = malloc(sizeof(long)*npts);
  long nc = npts/nelt;

  for (long i = 0; i < nelt; i++) {
    for (long j = 0; j < nc; j++) {
      u[nelt*i + j] = 1;
    }
  }

  gs(u, gs_long, gs_add, 0, *gsh, NULL);

  *weights = malloc(sizeof(long)*nelt);
  for (long i = 0; i < nelt; i++) {
    *(*weights + i) = 0;
    for (long j = 0; j < nc; j++) {
      *(*weights + i) += u[nelt*i + j];
    }
  }

  rsb_setup = 1;
}
//------------------------------------------------------------------------------
void ax(Vector *v, Vector *u, struct gs_data *gsh, long *weights, long nc) {
  if (rsb_setup == 0) {
    fprintf(stderr, "Need to call ax_setup before this routine.");
    return;
  }

  /* Asserts size of v = size of u
  */
  double *vv = v->vv;
  double *uv = u->vv;
  int size = v->size;

  printf("size = %d\n", size);
  printf("nc = %d\n", nc);

  double *ucv = malloc(sizeof(double)*nc*size);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < nc; j++) {
      ucv[size*i + j] = uv[i];
    }
  }

  gs(ucv, gs_double, gs_add, 0, gsh, NULL);

  for (int i = 0; i < size; i++) {
    vv[i] = weights[i]*uv[i];
    for (int j = 0; j < nc; j++) {
      vv[i] -= ucv[size*i + j];
    }
  }

}
//------------------------------------------------------------------------------
