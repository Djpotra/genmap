#include <stdio.h>
#include <stdlib.h>

void readmap(int *npts, int* glo_num, char* name) {
  FILE *fp;

  int nel, nactive, depth, d2, nrank, noutflow;
  int nc, jnk;

  fp = fopen(name, "r");
  if (fp == NULL) {
    fprintf(stderr, "Unable to open the file.\n");
  }

  fscanf(fp, "%d %d %d %d %d %d %d\n",
        &nel, &nactive, &depth, &d2, npts, &nrank, &noutflow);
//  printf("%d %d %d %d %d %d %d\n",
//          nel, nactive, depth, d2, npts, nrank, noutflow);

  nc  = *npts/nel;

  glo_num = malloc(sizeof(int)*(*npts));

  for (int i = 0; i < nel; i++) {
      fscanf(fp, "%d", &jnk);
      for (int j = 0; j < nc-1; j++) {
        fscanf(fp, "%d", &glo_num[i*nc + j]);
      }
      fscanf(fp, "%d\n", &glo_num[i*nc + nc - 1]);
  }

//  for (int i = 0; i < nel; i++) {
//      for (int j = 0; j < nc-1; j++) {
//        printf("%d ", glo_num[i*nc + j]);
//      }
//      printf("%d\n", glo_num[i*nc + nc - 1]);
//  }

  fclose(fp);
}
