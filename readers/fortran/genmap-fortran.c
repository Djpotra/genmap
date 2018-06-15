#include <genmap-readers.h>

#include <stdio.h>
//
// GenmapHandle: Create, Destroy
//
int GenmapCreateHandle_fortran(GenmapHandle h) {
  h->Read = GenmapRead_fortran;

  return 0;
}
