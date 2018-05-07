#include <genmap-impl.h>

int GenmapCreateComm(GenmapHandle h, GenmapComm *c,
                     GenmapCommExternal ce) {
  return h->CreateComm(c, ce);
}

int GenmapDestroyComm(GenmapHandle h, GenmapComm c) {
  return h->DestroyComm(c);
}

int GenmapNp(GenmapComm c) {
  return c->gsComm.np;
}

int GenmapId(GenmapComm c) {
  return c->gsComm.id;
}

int GenmapAx(GenmapHandle h, GenmapComm c, GenmapVector u,
             GenmapVector weights, GenmapVector v) {
  return h->Ax(h, c, u, weights, v);
}

int GenmapAxInit(GenmapHandle h, GenmapComm c, GenmapVector weights) {
  return h->AxInit(h, c, weights);
}

int GenmapGop(GenmapHandle h, GenmapComm c, GenmapScalar *v) {
  return h->Gop(c, v);
}
