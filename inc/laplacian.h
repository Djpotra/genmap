#ifndef _LAPLACIAN_H_
#define _LAPLACIAN_H_

#include "linalg.h"
#include "gswrapper.h"

extern int rsb_setup;

void ax_setup(struct gs_data **gsh, double **weights, struct comm *c, \
                 unsigned int npts, unsigned int nelt, long *glo_num);

void ax(Vector *v, Vector *u, struct gs_data *gsh, double *weights, long nc);

#endif
