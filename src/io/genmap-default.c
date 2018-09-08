#include <genmap-io.h>

#include <stdio.h>
//
// GenmapHandle: Create
//
int GenmapCreateHandle_default(GenmapHandle h) {
  h->Read = GenmapRead_default;

  return 0;
}
//
// Do File I/O in parallel
//
int GenmapRead_default(GenmapHandle h, void *data) {
  char *name = (char *)data;
#ifdef GENMAP_MPI
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status st;

  MPI_File_open(h->global->gsComm.c, name, MPI_MODE_RDONLY, MPI_INFO_NULL,
                &fh);

  MPI_File_seek(fh, 0, MPI_SEEK_SET);
  MPI_File_get_size(fh, &offset);
#else
  FILE *fp = fopen(name, "rb");
  if(fp == NULL) {
    printf("Unable to open the file.\n");
    exit(1);
  }
#endif

  GenmapInt *headerArray;
  GenmapMalloc(GENMAP_HEADER_SIZE, &headerArray);
#ifdef GENMAP_MPI
  MPI_File_read(fh, headerArray, GENMAP_HEADER_SIZE, MPI_INT, &st);
#else
  GenmapInt result = fread(headerArray, sizeof(GenmapInt),
                           GENMAP_HEADER_SIZE, fp);
#endif

  // nel, nactive, depth, d2, npts, nrank, noutflow
  h->header->nel = headerArray[GENMAP_NEL];
  h->header->npts = headerArray[GENMAP_NPTS];

  GenmapInt nel = headerArray[GENMAP_NEL];
  GenmapInt nv = headerArray[GENMAP_NPTS] / nel;
  GenmapInt lelt = nel / h->Np(h->global);

#ifdef GENMAP_MPI
  GenmapInt start = h->Id(h->global) * lelt * (2 * nv + 1);
  if(h->Id(h->global) == h->Np(h->global) - 1)
    lelt = nel - h->Id(h->global) * lelt;
#endif

  GenmapInt out[2][1], buf[2][1];
  comm_scan(out, &(h->global->gsComm), gs_int, gs_add, &lelt, 1, buf);

  h->header->nv = nv;
  h->header->ne = nv;
  h->header->ndim = 2;
  h->header->lelt = lelt;
  h->header->start = out[0][0];

  array_reserve(struct GenmapElement_private, &(h->elementArray), lelt);
  h->elementArray.n = lelt;
  GenmapElements elements = GenmapGetElements(h);

#ifdef GENMAP_MPI
  MPI_File_seek(fh, (GENMAP_HEADER_SIZE + start)*sizeof(GenmapInt),
                MPI_SEEK_SET);
#endif

  for(GenmapInt i = 0; i < lelt; i++) {
#ifdef GENMAP_MPI
    MPI_File_read(fh, &(elements[i].globalId), 1, MPI_INT, &st);
    MPI_File_read(fh, elements[i].vertices, nv, MPI_INT, &st);
    MPI_File_read(fh, elements[i].edges, nv, MPI_INT, &st);
#else
    result += fread(&(elements[i].globalId), sizeof(GenmapInt), 1, fp);
    result += fread(elements[i].vertices, sizeof(GenmapInt), nv, fp);
    result += fread(elements[i].edges, sizeof(GenmapInt), nv, fp);
#endif
  }

#ifdef GENMAP_MPI
  MPI_File_close(&fh);
#else
  fclose(fp);
#endif

  GenmapFree(headerArray);

  return 0;
}
