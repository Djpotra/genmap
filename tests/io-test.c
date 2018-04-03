#include <genmap-impl.h>

#include <stdio.h>
//
// Test IO
//
int TestIO1(GenmapHandle h) {
  char *name = "nbrhd/nbrhd.map.bin";

  GenmapRead_private(h, name);

  assert(h->header->nel == 8);
  assert(h->header->nactive == 15);
  assert(h->header->depth == 3);
  assert(h->header->d2 == 8);
  assert(h->header->npts == 32);
  assert(h->header->nrank == 15);
  assert(h->header->noutflow == 0);
  assert(h->header->nc == 4);

  GenmapInt elemFirst[4] = {4, 9, 12, 11};
  GenmapInt elemLast[4] = {7, 6, 5, 1};

  if(h->Id(h->global) == 0) {
    assert(h->elements->globalId[0] == 6);
    for(GenmapInt j = 0; j < h->header->nc; j++) {
      assert(h->elements->vertices[j] == elemFirst[j]);
    }
  }

  int nc = h->header->nc;
  int verticesIndex = nc * (h->header->lelt - 1);
  if(h->Id(h->global) == h->Np(h->global) - 1) {
    assert(h->elements->globalId[h->header->lelt - 1] == 1);
    for(GenmapInt j = 0; j < h->header->nc; j++) {
      assert(h->elements->vertices[verticesIndex + j] == elemLast[j]);
    }
  }

  return 0;
}

int main(int argc, char **argv) {
#ifdef MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD);
  TestIO1(h);
  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
}
