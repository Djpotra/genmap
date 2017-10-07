#include <stdio.h>

#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"

int rsb_setup = 0;
//------------------------------------------------------------------------------
void ax_setup(struct gs_data *gsh, struct comm *c, long nc, long nelt, long *glo_num)
{
  long npts = nc*nelt;

  gsh = gs_setup(glo_num, npts, c, 0, gs_crystal_router, 0);

  rsb_setup = 1;
}
//------------------------------------------------------------------------------
void ax(Vector *v, Vector *u) {
  if (rsb_setup == 0) {
    fprintf(stderr, "Need to call ax_setup before this routine."); 
    return;
  }

}
//------------------------------------------------------------------------------
