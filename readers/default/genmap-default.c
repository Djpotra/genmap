#include <genmap-readers.h>

#include <stdio.h>
//
// GenmapHandle: Create, Destroy
//
int GenmapCreateHandle_default(GenmapHandle h) {

  h->Read = GenmapRead_default;

  return 0;
}
