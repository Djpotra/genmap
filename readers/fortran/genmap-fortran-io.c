#include <genmap-readers.h>

#include <stdio.h>
//
// Do File I/O in parallel
//
int GenmapRead_fortran(GenmapHandle h, void *data) {
  GenmapInt  *vertices = (GenmapInt *)data;

  GenmapInt nel = h->header->nel;
  GenmapInt nv  = h->header->nv;
  GenmapInt lelt = nel / h->Np(h->global);

#ifdef MPI
  if(h->Id(h->global) == h->Np(h->global) - 1)
    lelt = nel - h->Id(h->global) * lelt;
#endif
  h->header->lelt = lelt;

  GenmapInt out[2][1], buf[2][1];
  comm_scan(out, &(h->global->gsComm), gs_int, gs_add, &lelt, 1, buf);
  h->header->start = out[0][0];

  array_reserve(struct GenmapElement_private, &(h->elementArray), lelt);
  h->elementArray.n = lelt;
  GenmapElements elements = GenmapGetElements(h);

  GenmapInt count = 0;
  GenmapInt start = h->header->start;
  for(GenmapInt i = 0; i < lelt; i++) {
    elements[i].globalId = start + i;
    for(GenmapInt j = 0; j < nv; j++) {
      elements[i].vertices[j] = vertices[count];
    }
  }

  return 0;
}
