#include <genmap-impl.h>

#ifdef GENMAP_MPI
#include <mpi.h>
#endif
#include <stdio.h>
//
// Test IO
//
int TestIO1(GenmapHandle h) {
  char *name = "mesh/cubeHexE8Thilina.msh";

  GenmapRead(h, name);

  assert(h->header->nel == 8);
  assert(h->header->ndim == 3);
  assert(h->header->nv == 8);
  assert(h->header->ne == 12);
  assert(h->header->Nnodes == 36);

  GenmapElements elements = GenmapGetElements(h);
  int e0[8] = {8, 7, 5, 6, 28, 27, 25, 26};
  int e1[8] = {12, 11, 9, 10, 4, 3, 1, 2};
  double v8[3] = {0.0, 0.0, 0.0};
  double v12[3] = {0., 0., 14.0};

  if(h->Id(h->global) == 0) {
    for(int i = 0; i < 8; i++)
      assert(e0[i] == elements[0].vertices[i]);

    assert(fabs(elements[0].x[0] - v8[0]) < GENMAP_TOL);
    assert(fabs(elements[0].y[0] - v8[1]) < GENMAP_TOL);
    assert(fabs(elements[0].z[0] - v8[2]) < GENMAP_TOL);
  }

  if(h->Id(h->global) == h->Np(h->global) - 1) {
    for(int i = 0; i < 8; i++)
      assert(e1[i] == elements[h->header->lelt - 1].vertices[i]);

    assert(fabs(elements[h->header->lelt-1].x[0] - v12[0]) < GENMAP_TOL);
    assert(fabs(elements[h->header->lelt-1].y[0] - v12[1]) < GENMAP_TOL);
    assert(fabs(elements[h->header->lelt-1].z[0] - v12[2]) < GENMAP_TOL);
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
  GenmapInit(&h, MPI_COMM_WORLD, "gmsh", 0);
  TestIO1(h);
  GenmapWrite(h, "cubeThilina");
  GenmapFinalize(h);

#ifdef GENMAP_MPI
  MPI_Finalize();
#endif
}
