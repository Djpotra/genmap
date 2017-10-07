#include <stdio.h>
#include <stdlib.h>

void readmap(int *npts, int* glo_num, char* name) {
  FILE *fp;

  int nel, nactive, depth, d2, nrank, noutflow;
  int nc, cnt, jnk;

  fp = fopen(name, "r");
  if (fp == NULL) {
    fprintf(stderr, "Unable to open the file.\n");
  }

  cnt = fscanf(fp, "%d %d %d %d %d %d %d\n",
        &nel, &nactive, &depth, &d2, npts, &nrank, &noutflow);
  if (cnt != 6) {
    fprintf(stderr, "Unable to read .rea file.\n");
  }

  nc  = *npts/nel;

  glo_num = malloc(sizeof(int)*(*npts));

  for (int i = 0; i < nel; i++) {
      cnt = fscanf(fp, "%d", &jnk);
      if (cnt != 1) {
        fprintf(stderr, "Unable to read .rea file.\n");
      }
      for (int j = 0; j < nc-1; j++) {
        cnt = fscanf(fp, "%d", &glo_num[i*nc + j]);
        if (cnt != 1) {
          fprintf(stderr, "Unable to read .rea file.\n");
        }
      }
      cnt = fscanf(fp, "%d\n", &glo_num[i*nc + nc - 1]);
      if (cnt != 1) {
        fprintf(stderr, "Unable to read .rea file.\n");
      }
  }

  fclose(fp);
}
