#include <genmap-impl.h>

GenmapInt GenmapNp_private(GenmapComm c) {
  return c->gsComm.np;
}

GenmapInt GenmapId_private(GenmapComm c) {
  return c->gsComm.id;
}
