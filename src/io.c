#include "io.h"

#include <stdio.h>
#include <stdlib.h>
//------------------------------------------------------------------------------
// TODO
void readmap_parallel(long *npts, long *nel, long **glo_num,
                                                        char* name) {
  MPI_Status status;
  MPI_File fh;
  MPI_Offset offset;

  int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  MPI_File_close(&fh);

  FILE *fp;
  long nactive, depth, d2, nrank, noutflow;
  long nc, cnt, jnk;

  fp = fopen(name, "r");
  if (fp == NULL) {
    fprintf(stderr, "Unable to open the file.\n");
  }

  cnt = fscanf(fp, "%ld %ld %ld %ld %ld %ld %ld\n",
        nel, &nactive, &depth, &d2, npts, &nrank, &noutflow);
  if (cnt != 7) {
    fprintf(stderr, "Unable to read .map file.\n");
  }

  nc  = *npts/(*nel);

  *glo_num = malloc(sizeof(long)*(*npts));

  for (long i = 0; i < *nel; i++) {
      cnt = fscanf(fp, "%ld", &jnk);
      if (cnt != 1) {
        fprintf(stderr, "Unable to read .map file.\n");
      }
      for (long j = 0; j < nc - 1; j++) {
        cnt = fscanf(fp, "%ld", *glo_num + (i*nc + j));
        if (cnt != 1) {
          fprintf(stderr, "Unable to read .map file.\n");
        }
      }
      cnt = fscanf(fp, "%ld\n", *glo_num + (i*nc + nc - 1));
      if (cnt != 1) {
        fprintf(stderr, "Unable to read .map file.\n");
      }
  }

  fclose(fp);
}
//------------------------------------------------------------------------------
void readmap(long *npts, long *nel, long **glo_num, char* name) {
  FILE *fp;

  long nactive, depth, d2, nrank, noutflow;
  long nc, cnt, jnk;

  fp = fopen(name, "r");
  if (fp == NULL) {
    fprintf(stderr, "Unable to open the file.\n");
  }

  cnt = fscanf(fp, "%ld %ld %ld %ld %ld %ld %ld\n",
        nel, &nactive, &depth, &d2, npts, &nrank, &noutflow);
  if (cnt != 7) {
    fprintf(stderr, "Unable to read .map file.\n");
  }

  nc  = *npts/(*nel);

  *glo_num = malloc(sizeof(long)*(*npts));

  for (long i = 0; i < *nel; i++) {
      cnt = fscanf(fp, "%ld", &jnk);
      if (cnt != 1) {
        fprintf(stderr, "Unable to read .map file.\n");
      }
      for (long j = 0; j < nc - 1; j++) {
        cnt = fscanf(fp, "%ld", *glo_num + (i*nc + j));
        if (cnt != 1) {
          fprintf(stderr, "Unable to read .map file.\n");
        }
      }
      cnt = fscanf(fp, "%ld\n", *glo_num + (i*nc + nc - 1));
      if (cnt != 1) {
        fprintf(stderr, "Unable to read .map file.\n");
      }
  }

  fclose(fp);
}
//------------------------------------------------------------------------------
