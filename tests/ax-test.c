#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif

#include <stdio.h>
//
// Test Ax
//
int TestAx1(GenmapHandle h, GenmapVector weights) {
  GenmapVector u, v, answer;
  GenmapInt lelt = h->header->lelt;

  GenmapCreateOnesVector(&u, lelt);
  GenmapCreateZerosVector(&answer, lelt);
  GenmapCreateOnesVector(&v, lelt);

  h->Ax(h, h->global, u, weights, v);

  assert(GenmapVectorsEqual(v, answer, GENMAP_TOL) == 1);

  GenmapDestroyVector(u);
  GenmapDestroyVector(v);
  GenmapDestroyVector(answer);

  return 0;
}

int TestAx2(GenmapHandle h, GenmapVector weights) {
  GenmapVector u, v, answer;
  GenmapInt lelt = h->header->lelt;
  GenmapInt nc = h->header->nc;

  GenmapCreateZerosVector(&u, lelt);
  if(h->Id(h->global) == 0)
    u->data[0] = 1.;
  GenmapCreateZerosVector(&answer, lelt);
  GenmapCreateOnesVector(&v, lelt);

  h->Ax(h, h->global, u, weights, v);
  printf("Ax2: %d v = ", h->Id(h->global));
  GenmapPrintVector(v);
  printf("\nAx2: %d weights = \n", h->Id(h->global));
  GenmapPrintVector(weights);
  printf("\n");

//  assert(GenmapVectorsEqual(v, answer, GENMAP_TOL) == 1);

  for(GenmapInt i = 0; i < lelt; i++) {
    printf("proc  : %d globalId: %d vertices: ", h->Id(h->global),
           h->elements->globalId[i]);
    for(GenmapInt j = 0; j < nc; j++) {
      printf("%d ", h->elements->vertices[i * nc + j]);
    }
    printf("\n");
  }

  GenmapDestroyVector(u);
  GenmapDestroyVector(v);
  GenmapDestroyVector(answer);

  return 0;
}

int main(int argc, char **argv) {
#ifdef MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD, "default");

  char *name = "mesh/box2D_2.bin";
  GenmapRead(h, name);

  GenmapVector weights;
  GenmapCreateVector(&weights, h->header->lelt);
  h->AxInit(h, h->global, weights);

  TestAx1(h, weights);
  TestAx2(h, weights);

  GenmapDestroyVector(weights);

  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
}
