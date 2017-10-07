#include <stdio.h>
#include <stdlib.h>

#include "io.h"
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
