#include <genmap-readers.h>

#include <stdio.h>
//
// Do File I/O in parallel
//
int GenmapRead_fortran(GenmapHandle h, void *data) {
  int  **dataPtr = (int **)data;

  GenmapInt nel  = dataPtr[0][0];
  GenmapInt nc   = dataPtr[0][1];
  GenmapInt lelt = nel / h->Np(h->global);

  printf("nel  = %d\n", nel);
  printf("nc   = %d\n", nc);
  printf("lelt = %d\n", lelt);

#ifdef MPI
  GenmapInt start = h->Id(h->global) * lelt * (2 * nc + 1);
  if(h->Id(h->global) == h->Np(h->global) - 1)
    lelt = nel - h->Id(h->global) * lelt;
#endif

//  GenmapInt out[2][1], buf[2][1];
//  comm_scan(out, &(h->global->gsComm), gs_int, gs_add, &lelt, 1, buf);
//
//  h->header->lelt = lelt;
//  h->header->start = out[0][0];
//
//  array_reserve(struct GenmapElement_private, &(h->elementArray), lelt);
//  h->elementArray.n = lelt;
//  GenmapElements elements = GenmapGetElements(h);

//  for(GenmapInt i = 0; i < lelt; i++) {
//  }

  return 0;
}
