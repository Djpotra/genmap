#include <genmap-readers.h>

#include <stdio.h>
//
// Do File I/O in parallel
//
int GenmapRead_fortran(GenmapHandle h, void *data) {
  GenmapInt  *vertices = (GenmapInt *)data;

  GenmapInt nel = h->header->nel;
  GenmapInt nv  = h->header->nv;
#ifdef DEBUG
  for(int i = 0; i < nel; i++) {
    for(int j = 0; j < nv; j++) {
//      printf("rvertices:%d\n",vertices[i*nv+j]);
    }
  }
#endif

  GenmapInt out[2][1], buf[2][1];
  comm_scan(out, &(h->global->gsComm), gs_int, gs_add, &nel, 1, buf);
  h->header->start = out[0][0];

  array_reserve(struct GenmapElement_private, &(h->elementArray), nel);
  h->elementArray.n = nel;
  h->header->lelt = nel;
  GenmapElements elements = GenmapGetElements(h);

  GenmapInt count = 0;
  GenmapInt start = h->header->start;
  for(GenmapInt i = 0; i < nel; i++) {
    elements[i].globalId = start + i;
    for(GenmapInt j = 0; j < nv; j++) {
      elements[i].vertices[j] = vertices[count];
      count++;
    }
  }

  return 0;
}
