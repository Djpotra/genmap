#include <genmap-impl.h>

#include <stdio.h>
//
// Do File I/O in parallel
//
int GenmapRead(GenmapHandle h, GenmapElement *elements,
               GenmapHeader mapheader, char* name) {
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

  GenmapInt32 *headerArray;
  GenmapMalloc(GENMAP_HEADER_SIZE, &headerArray);
#ifdef MPI
  MPI_File_read(fh, headerArray, GENMAP_HEADER_SIZE, MPI_INT, &st);
#else
  GenmapInt32 result = fread(headerArray, sizeof(GenmapInt32), GENMAP_HEADER_SIZE,
                             fp);
#endif

  // nel, nactive, depth, d2, npts, nrank, noutflow
  mapheader->nel = headerArray[GENMAP_NEL];
  mapheader->nactive = headerArray[GENMAP_NACTIVE];
  mapheader->depth = headerArray[GENMAP_DEPTH];
  mapheader->d2 = headerArray[GENMAP_D2];
  mapheader->npts = headerArray[GENMAP_NPTS];
  mapheader->nrank = headerArray[GENMAP_NRANK];
  mapheader->noutflow = headerArray[GENMAP_NOUTFLOW];

  GenmapInt32 nel = headerArray[GENMAP_NEL];
  GenmapInt32 nc = headerArray[GENMAP_NPTS] / nel;
  GenmapInt32 lelt = nel / h->globalComm.np;

#ifdef MPI
  GenmapInt32 start = h->globalComm.id * lelt * (nc + 1);
  if(h->globalComm.id == h->globalComm.np - 1)
    lelt = nel - h->globalComm.id * lelt;
#endif

  mapheader->nc = nc;
  mapheader->lelt = lelt;

  GenmapMalloc(lelt, elements);

#ifdef MPI
  MPI_File_seek(fh, (GENMAP_HEADER_SIZE + start)*sizeof(GenmapInt32),
                MPI_SEEK_SET);
#endif

  for(GenmapInt32 i = 0; i < lelt; i++) {
    GenmapElement elementi = *elements + i;
#ifdef MPI
    MPI_File_read(fh, &elementi->globalId,  1, MPI_INT, &st);
    MPI_File_read(fh, elementi->vertices, nc, MPI_INT, &st);
#else
    result += fread(&elementi->globalId, sizeof(GenmapInt32), 1, fp);
    result += fread(elementi->vertices, sizeof(GenmapInt32), nc, fp);
#endif
  }

#ifdef MPI
  MPI_File_close(&fh);
#endif

  free(headerArray);

  return 0;
}
