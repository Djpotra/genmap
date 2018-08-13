#include <genmap-io.h>

#include <stdio.h>
//
// GenmapHandle: Create
//
int GenmapCreateHandle_fortran(GenmapHandle h) {
  h->Read = GenmapRead_fortran;

  return 0;
}
//
// Do File I/O in parallel
//
int GenmapRead_fortran(GenmapHandle h, void *data) {
  GenmapInt  *vertices = (GenmapInt *)data;

  GenmapInt lelt = h->header->lelt;
  GenmapInt nv  = h->header->nv;
#ifdef GENMAP_DEBUG
  for(int i = 0; i < lelt; i++) {
    for(int j = 0; j < nv; j++) {
        printf("rvertices:%d\n",vertices[i*nv+j]);
    }
  }
#endif

  GenmapInt out[2][1], buf[2][1];
  comm_scan(out, &(h->global->gsComm), gs_int, gs_add, &lelt, 1, buf);
  h->header->start = out[0][0];
  h->header->nel = out[1][0];

  array_reserve(struct GenmapElement_private, &(h->elementArray), lelt);
  h->elementArray.n = lelt;
  h->header->lelt = lelt;

  GenmapElements elements = GenmapGetElements(h);

  GenmapInt count = 0;
  GenmapInt start = h->header->start + 1; // 1 for fortran index
  for(GenmapInt i = 0; i < lelt; i++) {
    elements[i].globalId = start + i;
    for(GenmapInt j = 0; j < nv; j++) {
      elements[i].vertices[j] = vertices[count];
      count++;
    }
  }

  return 0;
}
