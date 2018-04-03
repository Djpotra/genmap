#include <genmap-impl.h>

int GenmapCreateComm(GenmapComm *c) {
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
