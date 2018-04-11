#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
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

#ifdef DEBUG
  printf("%s:%d proc: %d v: ", __FILE__, __LINE__, h->Id(h->global));
  GenmapPrintVector(v);
  printf("\n");
#endif

  assert(GenmapVectorsEqual(v, answer, GENMAP_TOL) == 1);

  GenmapDestroyVector(u);
  GenmapDestroyVector(v);
  GenmapDestroyVector(answer);

  return 0;
}

int TestAx2(GenmapHandle h, GenmapVector weights) {
  GenmapVector u, v, answer;
  GenmapInt lelt = h->header->lelt;

  GenmapCreateZerosVector(&u, lelt);
  if(h->Id(h->global) == 0)
    u->data[0] = 1.;
  GenmapCreateOnesVector(&v, lelt);

  h->Ax(h, h->global, u, weights, v);

#ifdef DEBUG
  printf("%s:%d proc: %d v: ", __FILE__, __LINE__, h->Id(h->global));
  GenmapPrintVector(v);
  printf("\n");
#endif

#ifdef MPI
  assert(h->Np(h->global) == 2);
#endif

  GenmapCreateZerosVector(&answer, lelt);
  if(h->Id(h->global) == 0) {
    answer->data[0] = -3.;
    answer->data[1] = 1.;
    answer->data[2] = 1.;
    answer->data[3] = 1.;
  }

  assert(GenmapVectorsEqual(v, answer, GENMAP_TOL) == 1);

  GenmapDestroyVector(u);
  GenmapDestroyVector(v);
  GenmapDestroyVector(answer);

  return 0;
}

int TestAx3(GenmapHandle h, GenmapVector weights) {
  GenmapVector u, v, answer;
  GenmapInt lelt = h->header->lelt;

  GenmapCreateZerosVector(&u, lelt);
  if(h->Id(h->global) == 0)
    u->data[2] = 1.;
  GenmapCreateOnesVector(&v, lelt);

  h->Ax(h, h->global, u, weights, v);

#ifdef DEBUG
  printf("%s:%d proc: %d v: ", __FILE__, __LINE__, h->Id(h->global));
  GenmapPrintVector(v);
  printf("\n");
#endif

#ifdef MPI
  assert(h->Np(h->global) == 2);
#endif

  GenmapCreateOnesVector(&answer, lelt);
  if(h->Id(h->global) == 0) {
    answer->data[2] = -5.;
  }
  int index = 4;
#ifdef MPI
  index = 0;
  if(h->Id(h->global) == 1) {
#endif
    answer->data[index + 2] = 0.;
    answer->data[index + 3] = 0.;
#ifdef MPI
  }
#endif

  assert(GenmapVectorsEqual(v, answer, GENMAP_TOL) == 1);

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
  TestAx3(h, weights);

  GenmapDestroyVector(weights);

  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
}
