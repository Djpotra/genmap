#include <genmap-readers.h>

#include <stdio.h>
int GenmapCreateHandle_fortran(GenmapHandle h) {
  h->Read = GenmapRead_fortran;

  return 0;
}
