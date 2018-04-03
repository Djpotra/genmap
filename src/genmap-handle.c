#include <genmap-impl.h>
//
// GenmapHeader: Create, Destroy
//
int GenmapHeaderInit_private(GenmapHeader *h);
int GenmapDestroyHeader_private(GenmapHeader h);
//
// GenmapElements: Create, Destroy
//
int GenmapElementsInit_private(GenmapElements *e);
int GenmapDestroyElements_private(GenmapElements e);
//
// GenmapHandle_private: Create, Destroy
//
int GenmapHandleInit_private(GenmapHandle *h) {
  return 0;
}

int GenmapDestroyHandle_private(GenmapHandle h) {
  return 0;
}
//
// GenmapInit
//
int GenmapInit(GenmapHandle *h, GenmapCommExternal ce) {
  GenmapMalloc(1, h);

  GenmapCreateComm_private(&(*h)->global, ce);
  (*h)->local = NULL;

  GenmapMalloc(1, &(*h)->elements);
  GenmapMalloc(1, &(*h)->header);

  (*h)->Id = GenmapId_private;
  (*h)->Np = GenmapNp_private;
  (*h)->Ax = GenmapAx_private;

  return 0;
}
//
// GenmapFinalize
//
int GenmapFinalize(GenmapHandle h) {
  if(h->global)
    GenmapDestroyComm_private(h->global);
  if(h->local)
    GenmapDestroyComm_private(h->local);
  free(h->elements);
  free(h->header);
  free(h);
  h = NULL;

  return 0;
}
//
// TODO Malloc and Free
//
int GenmapMallocArray();
int GenmapCallocArray();
int GenmapReallocArray();
int GenmapFree();
