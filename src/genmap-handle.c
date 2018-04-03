#include <genmap-impl.h>
//
// GenmapHandle_private: Create, Destroy
//
int GenmapCreateHandle_private(GenmapHandle *h) {
  GenmapMalloc(1, h);

  (*h)->global = NULL;
  (*h)->local = NULL;
  (*h)->header = NULL;
  (*h)->elements = NULL;

  (*h)->Id = GenmapId_private;
  (*h)->Np = GenmapNp_private;
  (*h)->Ax = GenmapAx_private;
  (*h)->AxInit = GenmapAxInit_private;

  return 0;
}

int GenmapDestroyHandle_private(GenmapHandle h) {
  free(h);
  h = NULL;

  return 0;
}
