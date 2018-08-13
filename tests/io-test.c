#include <genmap-impl.h>

#ifdef GENMAP_MPI
#include <mpi.h>
#endif
#include <stdio.h>
//
// Test IO
//
int TestIO1(GenmapHandle h) {
  char *name = "mesh/box2D_2.bin";

  GenmapRead(h, name);
  GenmapElements elements = GenmapGetElements(h);

  assert(h->header->nel == 8);
  assert(h->header->nactive == 15);
  assert(h->header->depth == 3);
  assert(h->header->d2 == 8);
  assert(h->header->npts == 32);
  assert(h->header->nrank == 15);
  assert(h->header->noutflow == 0);
  assert(h->header->nv == 4);

  int nv = h->header->nv;
  GenmapInt verticesFirst[4] = {1, 4, 2, 5};
  GenmapInt edgesFirst[4] = {1, 4, 2, 3};
  GenmapInt verticesLast[4] = {11, 14, 12, 15};
  GenmapInt edgesLast[4] = {20, 22, 16, 21};

  if(h->Id(h->global) == 0) {
    assert(elements[0].globalId == 1);
    for(GenmapInt j = 0; j < nv; j++) {
      assert(elements[0].vertices[j] == verticesFirst[j]);
      assert(elements[0].edges[j] == edgesFirst[j]);
    }
  }

  int lelt = h->header->lelt - 1;
  if(h->Id(h->global) == h->Np(h->global) - 1) {
    assert(elements[lelt].globalId == 8);
    for(GenmapInt j = 0; j < nv; j++) {
      assert(elements[lelt].vertices[j] == verticesLast[j]);
      assert(elements[lelt].edges[j] == edgesLast[j]);
    }
  }

  return 0;
}

int main(int argc, char **argv) {
#ifdef GENMAP_MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD, "default", 0);
  TestIO1(h);
  GenmapFinalize(h);

#ifdef GENMAP_MPI
  MPI_Finalize();
#endif
}
