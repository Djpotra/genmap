#include <genmap-impl.h>
//
// GenmapHandle: Create, Destroy
//
int GenmapCreateHandle(GenmapHandle *h) {
  GenmapMalloc(1, h);

  (*h)->Create(h);

  return 0;
}

int GenmapDestroyHandle(GenmapHandle h) {
  h->Destroy(h);

  free(h);
  h = NULL;

  return 0;
}
