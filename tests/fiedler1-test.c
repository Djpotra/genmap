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
  int lelt = h->header->lelt;

  GenmapVector weights, u, v;
  GenmapCreateZerosVector(&weights, lelt);
  GenmapCreateOnesVector(&u, lelt);
  GenmapCreateOnesVector(&v, lelt);

  h->AxInit(h, h->global, weights);
  GenmapPrintVector(weights);
  printf("weights\n");
  h->Ax(h, h->global, u, weights, v);
  GenmapPrintVector(v);
  printf("1*laplacian\n");
  for(int i = 0; i < lelt; i++) {
    for(int j= 0; j < lelt; j++) {
      u->data[j] = 0.0;
      if(i == j) u->data[j] = 1.0;
    }
    h->Ax(h, h->global, u, weights, v);
    GenmapPrintVector(v); 
    printf("\n");
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
  GenmapFiedler(h, h->local, 1);
  GenmapFinalize(h);

#ifdef GENMAP_MPI
  MPI_Finalize();
#endif
}
