#include <stdio.h>
#include <mpi.h>

#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"

int rsb_setup = 0;
//------------------------------------------------------------------------------
void ax_setup(struct gs_data *gsh, long **weights, struct comm *c, \
                            long npts, long nelt, long *glo_num)
{
  gsh = gs_setup(glo_num, npts, c, 0, gs_crystal_router, 0);

  long *u = malloc(sizeof(long)*npts);
  long nc = npts/nelt;

  for (long i = 0; i < nelt; i++) {
    for (long j = 0; j < nc; j++) {
      u[nelt*i + j] = 1;
    }
  }

  gs(u, gs_long, gs_add, 0, gsh, NULL);

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
void ax(Vector *v, Vector *u, struct gs_data *gsh) {
  if (rsb_setup == 0) {
    fprintf(stderr, "Need to call ax_setup before this routine.");
    return;
  }
}
//------------------------------------------------------------------------------
