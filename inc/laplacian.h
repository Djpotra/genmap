#ifdef _LAPLACIAN_H_
#define _LAPLACIAN_H

#include "linalg.h"
#include "gswrapper.h"

extern int rsb_setup;

void ax_setup(struct gs_data *gsh, long npts, long nelt, long *glo_num);

void ax(Vector *v, Vector *u);

#endif
