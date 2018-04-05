#include <genmap.h>
//
// GenmapHandle: Create, Destroy
//
int GenmapCreateHandle_default(GenmapHandle *h) {
  GenmapMalloc(1, h);

  (*h)->global = NULL;
  (*h)->local = NULL;
  (*h)->header = NULL;
  (*h)->elements = NULL;

  (*h)->Id = GenmapId_default;
  (*h)->Np = GenmapNp_default;
  (*h)->Ax = GenmapAx_default;
  (*h)->AxInit = GenmapAxInit_default;
  (*h)->Gop = GenmapGop_default;
  (*h)->Read = GenmapRead_default;

  return 0;
}

int GenmapDestroyHandle_default(GenmapHandle h) {
  free(h);
  h = NULL;

  return 0;
}
