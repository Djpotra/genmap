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

int GenmapAxInit_private(GenmapVector v, GenmapVector u, GenmapHandle h,
                         GenmapComm c) {
//  int numPoints = h->mapHeader->lelt;
//  GenmapInt *globalId;
//  GenmapMalloc(numPoints, &globalNum);
//  c->gsHandle = gs_setup(glo_num, numPoints, &c->gsComm, 0, gs_auto, 0);
  return 0;
}
