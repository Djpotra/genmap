#include <default.h>

#include <stdio.h>
//
// GenmapHandle: Create, Destroy
//
int GenmapCreateHandle_default(GenmapHandle h) {
  h->global = NULL;
  h->local = NULL;
  h->header = NULL;
  h->elements = NULL;

  h->Id = GenmapId_default;
  h->Np = GenmapNp_default;
  h->Ax = GenmapAx_default;
  h->AxInit = GenmapAxInit_default;
  h->Gop = GenmapGop_default;
  h->Read = GenmapRead_default;

  return 0;
}

int GenmapDestroyHandle_default(GenmapHandle h) {
  return 0;
}

__attribute__((constructor))
static void Register(void) {                                            
  printf("Inside register\n");
  GenmapRegisterReader("default", GenmapCreateHandle_default);                          
}       
