#include <genmap-readers.h>

#include <stdio.h>
//
// Do File I/O in parallel
//
int GenmapRead_fortran(GenmapHandle h, void *data) {
  int  **headerArray = (int **)data;

  // nel, nactive, depth, d2, npts, nrank, noutflow
  h->header->nel     = headerArray[0][GENMAP_NEL];
  h->header->nactive = headerArray[0][GENMAP_NACTIVE];
  h->header->depth   = headerArray[0][GENMAP_DEPTH];
  h->header->d2      = headerArray[0][GENMAP_D2];
  h->header->npts    = headerArray[0][GENMAP_NPTS];
  h->header->nrank   = headerArray[0][GENMAP_NRANK];
  h->header->noutflow = headerArray[0][GENMAP_NOUTFLOW];

  GenmapInt nel = headerArray[0][GENMAP_NEL];
  GenmapInt nc = headerArray[0][GENMAP_NPTS] / nel;
  GenmapInt lelt = nel / h->Np(h->global);

#ifdef MPI
//  GenmapInt start = h->Id(h->global) * lelt * (2 * nc + 1);
  if(h->Id(h->global) == h->Np(h->global) - 1)
    lelt = nel - h->Id(h->global) * lelt;
#endif

  GenmapInt out[2][1], buf[2][1];
  comm_scan(out, &(h->global->gsComm), gs_int, gs_add, &lelt, 1, buf);

  h->header->nc = nc;
  h->header->lelt = lelt;
  h->header->start = out[0][0];

  array_reserve(struct GenmapElement_private, &(h->elementArray), lelt);
  h->elementArray.n = lelt;
//  GenmapElements elements = GenmapGetElements(h);

  for(GenmapInt i = 0; i < lelt; i++) {
  }

  return 0;
}
