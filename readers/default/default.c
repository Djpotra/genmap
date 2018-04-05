#include <genmap.h>
//
// GenmapHandle_default: Create, Destroy
//
int GenmapCreateHandle_default(GenmapHandle *h) {
  GenmapMalloc(1, h);

  (*h)->global = NULL;
  (*h)->local = NULL;
  (*h)->header = NULL;
  (*h)->elements = NULL;

  (*h)->Id = GenmapId_private;
  (*h)->Np = GenmapNp_private;
  (*h)->Ax = GenmapAx_private;
  (*h)->AxInit = GenmapAxInit_private;
  (*h)->Gop = GenmapGop_private;
  (*h)->Read = GenmapRead_private;

  return 0;
}

int GenmapDestroyHandle_default(GenmapHandle h) {
  free(h);
  h = NULL;

  return 0;
}
