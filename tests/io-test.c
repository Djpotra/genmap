#include <genmap-impl.h>

#include <stdio.h>
//
// Test IO
//
int TestIO1(GenmapHandle h) {
  char *name = "nbrhd/nbrhd.map.bin";
  GenmapElement elements;
  GenmapHeader mapheader;

  GenmapMalloc(1, &mapheader);
  GenmapRead(h, &elements, mapheader, name);

  free(elements);
  free(mapheader);

  return 0;
}

int TestIO2(GenmapHandle h) {
  char *name = "nbrhd/nbrhd.map.bin";
  GenmapElement elements;
  GenmapHeader mapheader;

  GenmapMalloc(1, &mapheader);
  GenmapRead(h, &elements, mapheader, name);

  for(GenmapInt32 i = 0; i < 1; i++) {
    printf("%d ", mapheader->nel);
    printf("%d ", mapheader->nactive);
    printf("%d ", mapheader->depth);
    printf("%d ", mapheader->d2);
    printf("%d ", mapheader->npts);
    printf("%d ", mapheader->nrank);
    printf("%d ", mapheader->noutflow);
    printf("%d ", mapheader->nc);
    printf("%d\n", mapheader->lelt);
  }

  for(GenmapInt32 i = 0;  i < mapheader->lelt; i++) {
    printf("%d ", elements[i].globalId);
    for(GenmapInt32 j = 0; j < mapheader->nc; j++) {
      printf("%d ", elements[i].vertices[j]);
    }
    printf("\n");
  }

  free(elements);
  free(mapheader);

  return 0;
}

int main(int argc, char **argv) {
  GenmapHandle h;
  GenmapInit(&h, argc, argv);
  TestIO1(h);
  TestIO2(h);
  GenmapFinalize(h);
}
