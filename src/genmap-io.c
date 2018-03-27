#include <genmap-impl.h>

#include <stdio.h>
//
// Do File I/O in parallel
//
int GenmapReadMPI(GenmapHandle h, GenmapElement *elements,
                  GenmapHeader mapheader, char* name) {
#ifdef MPI
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status st;

  MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

  MPI_File_seek(fh, 0, MPI_SEEK_SET);
  MPI_File_get_size(fh, &offset);

  GenmapInt32 *headerArray;
  GenmapMalloc(GENMAP_HEADER_SIZE, &headerArray);
  MPI_File_read(fh, headerArray, GENMAP_HEADER_SIZE, MPI_INT, &st);

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
  GenmapInt32 lelt = nel / h->c.np;

  GenmapInt32 start = h->c.id * lelt * (nc + 1);
  if(h->c.id == h->c.np - 1) lelt = nel - h->c.id * lelt;

  mapheader->nc = nc;
  mapheader->lelt = lelt;

  GenmapMalloc(lelt, elements);
  GenmapInt32 vertices[nc];

  MPI_File_seek(fh, (GENMAP_HEADER_SIZE + start)*sizeof(GenmapInt32),
                MPI_SEEK_SET);
  GenmapInt32 gid;
  for(GenmapInt32 i = 0; i < lelt; i++) {
    GenmapElement elementi = *elements + i;
    MPI_File_read(fh, &gid,  1, MPI_INT, &st);
    elementi->globalId = gid;

    MPI_File_read(fh, vertices, nc, MPI_INT, &st);
    for(GenmapInt32 j = 0; j < nc; j++) {
      elementi->vertices[j] = vertices[j];
    }
  }

  MPI_File_close(&fh);
  free(headerArray);
#endif
  return 0;
}
//
// Do File I/O in Serial
//
int GenmapReadSerial(GenmapHandle h, GenmapElement *elements,
                     GenmapHeader mapheader, char* name) {
  FILE *fp = fopen(name, "rb");
  if(fp == NULL) {
    printf("Unable to open the file.\n");
    exit(1);
  }

  GenmapInt32 *headerArray;
  GenmapMalloc(GENMAP_HEADER_SIZE, &headerArray);
  GenmapInt32 result = fread(headerArray, sizeof(GenmapInt32), GENMAP_HEADER_SIZE,
                             fp);

  // nel, nactive, depth, d2, npts, nrank, noutflow
  mapheader->nel = headerArray[GENMAP_NEL];
  mapheader->nactive = headerArray[GENMAP_NACTIVE];
  mapheader->depth = headerArray[GENMAP_DEPTH];
  mapheader->d2 = headerArray[GENMAP_D2];
  mapheader->npts = headerArray[GENMAP_NPTS];
  mapheader->nrank = headerArray[GENMAP_NRANK];
  mapheader->noutflow = headerArray[GENMAP_NOUTFLOW];

  GenmapInt32 nc = headerArray[GENMAP_NPTS] / headerArray[GENMAP_NEL];
  GenmapInt32 nel = headerArray[GENMAP_NEL];

  mapheader->nc = nc;
  mapheader->lelt = nel;

  GenmapMalloc(nel, elements);
  GenmapInt32 vertices[nc];

  for(GenmapInt32 i = 0; i < nel; i++) {
    GenmapElement elementi = *elements + i;
    result += fread(&elementi->globalId, sizeof(GenmapInt32), 1, fp);

    result += fread(vertices, sizeof(GenmapInt32), nc, fp);
    for(GenmapInt32 j = 0; j < nc; j++) {
      elementi->vertices[j] = vertices[j];
    }
  }

  free(headerArray);
  fclose(fp);
  return 0;
}
//
// GenmapRead Function
//
int GenmapRead(GenmapHandle h, GenmapElement *elements,
               GenmapHeader mapheader, char* name) {
#ifdef MPI
  return GenmapReadMPI(h, elements, mapheader, name);
#else
  return GenmapReadSerial(h, elements, mapheader, name);
#endif
}
