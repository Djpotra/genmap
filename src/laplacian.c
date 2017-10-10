#include <stdio.h>
#include <mpi.h>

#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"

int rsb_setup = 0;
//------------------------------------------------------------------------------
void ax_setup(struct gs_data **gsh, int **weights, struct comm *c, \
                 unsigned int npts, unsigned int nelt, long *glo_num)
{
  *gsh = gs_setup(glo_num, npts, c, 1, gs_all_reduce, 0);

  int *u = malloc(sizeof(int)*npts);
  int nc = npts/nelt;

//  printf("npts = %u\n", npts);
//  printf("nelt = %u\n", nelt);
//  printf("nc = %d\n", nc);

  for (unsigned int i = 0; i < nelt; i++) {
    for (int j = 0; j < nc; j++) {
      u[nelt*i + j] = 1;
    }
  }

//  for (unsigned int i = 0; i < nelt; i++) {
//    for (int j = 0; j < nc; j++) {
//      printf("u = %d\n", u[nelt*i + j]);
//    }
//  }

  gs(u, gs_int, gs_add, 0, *gsh, NULL);

  *weights = malloc(sizeof(int)*nelt);
  for (long i = 0; i < nelt; i++) {
    *(*weights + i) = 0;
    for (long j = 0; j < nc; j++) {
//      printf("u = %d\n", u[nelt*i + j]);
      *(*weights + i) += u[nelt*i + j];
    }
  }

//  for (int i = 0; i < nelt; i++) {
//    printf("lelt = %d, weight = %ld\n", i, (*weights)[i]);
//  }
  rsb_setup = 1;
}
//------------------------------------------------------------------------------
void ax(Vector *v, Vector *u, struct gs_data *gsh, int *weights, long nc) {
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
