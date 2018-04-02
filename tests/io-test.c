#include <genmap-impl.h>

#include <stdio.h>
//
// Test IO
//
int TestIO1(GenmapHandle h) {
  char *name = "nbrhd/nbrhd.map.bin";
  GenmapElement elements;
  GenmapHeader mapHeader;

  GenmapMalloc(1, &mapHeader);
  GenmapRead(h, &elements, mapHeader, name);

  assert(mapHeader->nel == 8);
  assert(mapHeader->nactive == 15);
  assert(mapHeader->depth == 3);
  assert(mapHeader->d2 == 8);
  assert(mapHeader->npts == 32);
  assert(mapHeader->nrank == 15);
  assert(mapHeader->noutflow == 0);
  assert(mapHeader->nc == 4);

  GenmapInt elemFirst[4] = {4, 9, 12, 11};
  GenmapInt elemLast[4] = {7, 6, 5, 1};

  if(h->Id(h->global) == 0) {
    assert(elements[0].globalId == 6);
    for(GenmapInt j = 0; j < mapHeader->nc; j++) {
      assert(elements[0].vertices[j] == elemFirst[j]);
    }
  }

  if(h->Id(h->global) == h->Np(h->global) - 1) {
    assert(elements[mapHeader->lelt - 1].globalId == 1);
    for(GenmapInt j = 0; j < mapHeader->nc; j++) {
      assert(elements[mapHeader->lelt - 1].vertices[j] == elemLast[j]);
    }
  }

  // TODO Create separate Free functions
  free(elements);
  free(mapHeader);

  return 0;
}

int main(int argc, char **argv) {
  GenmapHandle h;
  GenmapInit(&h, argc, argv);
  TestIO1(h);
  GenmapFinalize(h);
}
