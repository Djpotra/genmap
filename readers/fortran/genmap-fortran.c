#include <genmap-readers.h>

#include <stdio.h>
//
// GenmapHandle: Create, Destroy
//
int GenmapCreateHandle_fortran(GenmapHandle h) {
  h->global = NULL;
  h->local = NULL;
  h->CreateComm = GenmapCreateComm_fortran;
  h->DestroyComm = GenmapDestroyComm_fortran;

  h->header = NULL;
  h->CreateHeader = GenmapCreateHeader_fortran;
  h->DestroyHeader = GenmapDestroyHeader_fortran;

  h->elementArray.ptr = NULL;
  h->elementArray.n = h->elementArray.max = 0;
  h->GetElements = GenmapGetElements_fortran;
  h->CreateElements = GenmapCreateElements_fortran;
  h->DestroyElements = GenmapDestroyElements_fortran;

  h->Id = GenmapId_fortran;
  h->Np = GenmapNp_fortran;

  h->Ax = GenmapAx_fortran;
  h->AxInit = GenmapAxInit_fortran;

  h->Gop = GenmapGop_fortran;
  h->Read = GenmapRead_fortran;

  h->Destroy = GenmapDestroyHandle_fortran;

  return 0;
}

int GenmapDestroyHandle_fortran(GenmapHandle h) {
  return 0;
}
