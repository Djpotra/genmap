#include "io.h"
#include "mpiwrapper.h"

//------------------------------------------------------------------------------
#ifdef MPI
void readmap_mpi(struct comm *c, struct element **elements,
                 struct header *mapheader, char* name) {
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status st;

  MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

  MPI_File_seek(fh, 0, MPI_SEEK_SET);
  MPI_File_get_size(fh, &offset);

  int32 *headerArray = malloc(sizeof(int32)*MAP_HEADER_SIZE);
  MPI_File_read(fh, headerArray, MAP_HEADER_SIZE, MPI_INT, &st);
  // nel, nactive, depth, d2, npts, nrank, noutflow
  mapheader->nel      = headerArray[NEL];
  mapheader->nactive  = headerArray[NACTIVE];
  mapheader->depth    = headerArray[DEPTH];
  mapheader->d2       = headerArray[D2];
  mapheader->npts     = headerArray[NPTS];
  mapheader->nrank    = headerArray[NRANK];
  mapheader->noutflow = headerArray[NOUTFLOW];

  int32 nel = headerArray[NEL];
  int32 nc  = headerArray[NPTS]/nel;
  int32 lelt =  nel/c->np;

  int32 start = c->id*lelt*(nc + 1);
  if (c->id == c->np - 1) lelt = nel - c->id*lelt;

  mapheader->nc       = nc;
  mapheader->lelt     = lelt;

  *elements = malloc(sizeof(struct element)*lelt);
  int32 vertices[nc];

  MPI_File_seek(fh, (MAP_HEADER_SIZE + start)*sizeof(int32), MPI_SEEK_SET);
  int32 gid;
  for (int32 i = 0; i < lelt; i++) {
    struct element *elementi = *elements + i;
    MPI_File_read(fh, &gid,  1, MPI_INT, &st);
    elementi->globalId = gid;

    MPI_File_read(fh, vertices, nc, MPI_INT, &st);
    for (int32 j = 0; j < nc; j++) {
      elementi->vertices[j] = vertices[j];
    }
  }

  MPI_File_close(&fh);
}
#endif

//------------------------------------------------------------------------------
void readmap_serial(struct element **elements, struct header *mapheader,
                    char* name) {
  FILE *fp = fopen(name, "rb");
  if (fp == NULL) {
    printf("Unable to open the file.\n");
    exit(1);
  }

  int32 *headerArray = malloc(sizeof(int32)*MAP_HEADER_SIZE);
  int32 result = fread(headerArray, sizeof(int32), MAP_HEADER_SIZE, fp);

  // nel, nactive, depth, d2, npts, nrank, noutflow
  mapheader->nel      = headerArray[NEL];
  mapheader->nactive  = headerArray[NACTIVE];
  mapheader->depth    = headerArray[DEPTH];
  mapheader->d2       = headerArray[D2];
  mapheader->npts     = headerArray[NPTS];
  mapheader->nrank    = headerArray[NRANK];
  mapheader->noutflow = headerArray[NOUTFLOW];

  int32 nc  = headerArray[NPTS]/headerArray[NEL];
  int32 nel = headerArray[NEL];

  mapheader->nc   = nc;
  mapheader->lelt = nel;

  *elements = malloc(sizeof(struct element)*nel);
  int32 vertices[nc];

  for (int32 i = 0; i < nel; i++) {
    struct element *elementi = elements[i];
    result += fread(&elementi->globalId, sizeof(int32), 1, fp);

    result += fread(vertices, sizeof(int32), nc, fp);
    for (int32 j = 0; j < nc; j++) {
      elementi->vertices[j] = vertices[j];
    }
  }

  fclose(fp);
}

//------------------------------------------------------------------------------
void readmap(struct comm *c, struct element **elements,
             struct header *mapheader, char* name) {
#ifdef MPI
  readmap_mpi   (c, elements, mapheader, name);
#else
  readmap_serial(elements, mapheader, name);
#endif
}
//------------------------------------------------------------------------------
