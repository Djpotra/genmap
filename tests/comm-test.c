#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
//
// Test Gop
//
int TestGop1(GenmapHandle h) {
  GenmapScalar u = (GenmapScalar) h->Id(h->global);
  GenmapInt n = h->Np(h->global);

  h->Gop(h->global, &u, 1, GENMAP_SUM);
  assert(fabs(u - 0.5 * n * (n - 1)) < GENMAP_TOL);

  return 0;
}
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

  GenmapCreateZerosVector(&answer, lelt);

#ifdef MPI
  if(h->Np(h->global) == 2) {
    if(h->Id(h->global) == 0) {
      answer->data[0] = -3.;
      answer->data[1] = 1.;
      answer->data[2] = 1.;
      answer->data[3] = 1.;
    }

    assert(GenmapVectorsEqual(v, answer, GENMAP_TOL) == 1);
  }
#endif


  GenmapDestroyVector(u);
  GenmapDestroyVector(v);
  GenmapDestroyVector(answer);

  return 0;
}

int TestAx3(GenmapHandle h, GenmapVector weights) {
#ifdef MPI
  GenmapVector u, v, answer;
  GenmapInt lelt = h->header->lelt;

  GenmapCreateZerosVector(&u, lelt);
  GenmapCreateOnesVector(&v, lelt);

  if(h->Np(h->global) == 2) {
    if(h->Id(h->global) == 0)
      u->data[2] = 1.;

    h->Ax(h, h->global, u, weights, v);

#ifdef DEBUG
    printf("%s:%d proc: %d v: ", __FILE__, __LINE__, h->Id(h->global));
    GenmapPrintVector(v);
    printf("\n");
#endif

    GenmapCreateOnesVector(&answer, lelt);

    if(h->Id(h->global) == 0) {
      answer->data[2] = -5.;
    }

    if(h->Id(h->global) == 1) {
      answer->data[2] = 0.;
      answer->data[3] = 0.;
    }

  } else if(h->Np(h->global) == 4) {
    if(h->Id(h->global) == 0)
      u->data[1] = 1.;

    h->Ax(h, h->global, u, weights, v);

#ifdef DEBUG
    printf("%s:%d proc: %d v: ", __FILE__, __LINE__, h->Id(h->global));
    GenmapPrintVector(v);
    printf("\n");
#endif

    GenmapCreateZerosVector(&answer, lelt);

    if(h->Id(h->global) == 0) {
      answer->data[0] = 1.;
      answer->data[1] = -3.;
    }

    if(h->Id(h->global) == 1) {
      answer->data[0] = 1.;
      answer->data[1] = 1.;
    }

  }
  assert(GenmapVectorsEqual(v, answer, GENMAP_TOL) == 1);
#endif

#ifdef MPI
  GenmapDestroyVector(u);
  GenmapDestroyVector(v);
  GenmapDestroyVector(answer);
#endif

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

  TestGop1(h);

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
