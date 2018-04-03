#include <genmap-impl.h>

int GenmapCreateComm_private(GenmapComm *c, GenmapCommExternal ce) {
  GenmapMalloc(1, c);
  comm_init(&(*c)->gsComm, ce);
  (*c)->gsHandle = NULL;
  (*c)->laplacianWeights = NULL;
  return 0;
}

int GenmapDestroyComm_private(GenmapComm c) {
  comm_free(&c->gsComm);
  if(c->gsHandle)
    gs_free(c->gsHandle);
  if(c->laplacianWeights)
    free(c->laplacianWeights);
  free(c);
  c = NULL;
  return 0;
}

int GenmapNp_private(GenmapComm c) {
  return c->gsComm.np;
}

int GenmapId_private(GenmapComm c) {
  return c->gsComm.id;
}

int GenmapAx_private(GenmapVector v, GenmapVector u, GenmapHandle h,
                     GenmapComm c) {
  return 0;
}

int GenmapAxInit_private(GenmapVector weights, GenmapHandle h,
                         GenmapComm c) {
  GenmapInt lelt = h->header->lelt;
  GenmapInt nc = h->header->nc;
  GenmapInt numPoints = nc * lelt;

  c->gsHandle = gs_setup(h->elements->globalId, numPoints, &c->gsComm, 0, gs_auto,
                         0);

  double *u;
  GenmapMalloc(numPoints, &u);

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nc; j++)
      u[nc * i + j] = 1;

  gs(u, gs_double, gs_add, 1, c->gsHandle, NULL);

  assert(weights->size == lelt);

  for(GenmapInt i = 0; i < lelt; i++) {
    weights->data[i] = 0;
    for(GenmapInt j = 0; j < nc; j++) {
      weights->data[i] += u[nc * i + j];
    }
  }

  return 0;
}
