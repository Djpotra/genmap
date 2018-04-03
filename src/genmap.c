#include <genmap-impl.h>
//
// GenmapInit
//
int GenmapInit(GenmapHandle *h, GenmapCommExternal ce) {
  GenmapCreateHandle_private(h);

  GenmapCreateComm_private(&(*h)->global, ce);

  (*h)->local = NULL;

  GenmapCreateElements_private(&(*h)->elements);
  GenmapCreateHeader_private(&(*h)->header);

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
  GenmapDestroyElements_private(h->elements);
  GenmapDestroyHeader_private(h->header);
  GenmapDestroyHandle_private(h);

  return 0;
}
//
// TODO Malloc and Free
//
int GenmapMallocArray();
int GenmapCallocArray();
int GenmapReallocArray();
int GenmapFree();
