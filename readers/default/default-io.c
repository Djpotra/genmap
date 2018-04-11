#include <default.h>

#include <stdio.h>
//
// GenmapHeader: Create, Destroy
//
int GenmapCreateHeader_default(GenmapHeader *h) {
  GenmapMalloc(1, h);

  return 0;
}

int GenmapDestroyHeader_default(GenmapHeader h) {
  free(h);
  h = NULL;

  return 0;
}
//
// GenmapElements: Create, Destroy
//
int GenmapCreateElements_default(GenmapElements *e) {
  GenmapMalloc(1, e);

  (*e)->globalId = NULL;
  (*e)->vertices = NULL;
  (*e)->edges = NULL;
  (*e)->fiedler = NULL;

  return 0;
}

int GenmapDestroyElements_default(GenmapElements e) {
  if(e->globalId)
    free(e->globalId);
  if(e->vertices)
    free(e->vertices);
  if(e->edges)
    free(e->edges);
  if(e->fiedler)
    free(e->fiedler);
  free(e);
  e = NULL;
  return 0;
}
//
// Do File I/O in parallel
//
int GenmapRead_default(GenmapHandle h, char *name) {
#ifdef MPI
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status st;

  MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

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
#ifdef MPI
  MPI_File_read(fh, headerArray, GENMAP_HEADER_SIZE, MPI_INT, &st);
#else
  GenmapInt result = fread(headerArray, sizeof(GenmapInt), GENMAP_HEADER_SIZE,
                           fp);
#endif

  // nel, nactive, depth, d2, npts, nrank, noutflow
  h->header->nel = headerArray[GENMAP_NEL];
  h->header->nactive = headerArray[GENMAP_NACTIVE];
  h->header->depth = headerArray[GENMAP_DEPTH];
  h->header->d2 = headerArray[GENMAP_D2];
  h->header->npts = headerArray[GENMAP_NPTS];
  h->header->nrank = headerArray[GENMAP_NRANK];
  h->header->noutflow = headerArray[GENMAP_NOUTFLOW];

  GenmapInt nel = headerArray[GENMAP_NEL];
  GenmapInt nc = headerArray[GENMAP_NPTS] / nel;
  GenmapInt lelt = nel / h->Np(h->global);

#ifdef MPI
  GenmapInt start = h->Id(h->global) * lelt * (2*nc + 1);
  if(h->Id(h->global) == h->Np(h->global) - 1)
    lelt = nel - h->Id(h->global) * lelt;
#endif

  h->header->nc = nc;
  h->header->lelt = lelt;

  GenmapMalloc(lelt, &(h->elements->globalId));
  GenmapMalloc(lelt, &(h->elements->fiedler));
  GenmapMalloc(lelt * nc, &(h->elements->vertices));
  GenmapMalloc(lelt * nc, &(h->elements->edges));
#ifdef MPI
  MPI_File_seek(fh, (GENMAP_HEADER_SIZE + start)*sizeof(GenmapInt),
                MPI_SEEK_SET);
#endif

  for(GenmapInt i = 0; i < lelt; i++) {
#ifdef MPI
    MPI_File_read(fh, h->elements->globalId + i,  1, MPI_INT, &st);
    MPI_File_read(fh, h->elements->vertices + i * nc, nc, MPI_INT, &st);
    MPI_File_read(fh, h->elements->edges + i * nc, nc, MPI_INT, &st);
#else
    result += fread(h->elements->globalId + i, sizeof(GenmapInt), 1, fp);
    result += fread(h->elements->vertices + i * nc, sizeof(GenmapInt), nc, fp);
    result += fread(h->elements->edges + i * nc, sizeof(GenmapInt), nc, fp);
#endif
  }

#ifdef MPI
  MPI_File_close(&fh);
#else
  fclose(fp);
#endif

  free(headerArray);

  return 0;
}
