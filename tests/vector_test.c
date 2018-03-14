#include <genmap-impl.h>

#include <math.h>
//------------------------------------------------------------------------------
// Test GenmapVector
//
void TestGenmapCreateVector() {
  GenmapVector x, y;

  GenmapCreateVector(&x, 20);
  GenmapCreateVector(&y, 10);

  assert(x->size == 20);
  assert(y->size == 10);

  GenmapDestroyVector(x);
  GenmapDestroyVector(y);
}

void TestGenmapSetVector() {
  GenmapVector x;

  GenmapCreateVector(&x, 5);

  GenmapScalar dx[5] = {0., 1., 2., 3., 4.};

  GenmapSetVector(x, dx);

  for(int i = 0; i < 5; i++)
    assert(fabs(x->data[i] - i) < GENMAP_TOL);

  GenmapDestroyVector(x);
}

void TestGenmapVectorsEqual() {
  GenmapVector x, y, z;

  GenmapCreateVector(&x, 5);
  GenmapCreateVector(&y, 5);
  GenmapCreateVector(&z, 5);

  GenmapScalar dx[5] = {0., 0., 0., 0., 0.};
  GenmapScalar dy[5] = {0., 0., 0., 0., 0.};
  GenmapScalar dz[5] = {0., 0., 0., 0., 1.};

  GenmapSetVector(x, dx);
  GenmapSetVector(y, dy);
  GenmapSetVector(z, dz);

  assert(GenmapVectorsEqual(x, y, GENMAP_TOL));
  assert(!GenmapVectorsEqual(y, z, GENMAP_TOL));

  GenmapDestroyVector(x);
  GenmapDestroyVector(y);
  GenmapDestroyVector(z);
}

void TestGenmapCopyVector() {
  GenmapVector x, y;

  GenmapCreateVector(&x, 5);
  GenmapCreateVector(&y, 5);

  GenmapScalar dx[5] = {1., 2., 3., 4., 5.};
  GenmapSetVector(x, dx);

  GenmapCopyVector(y, x);

  assert(GenmapVectorsEqual(x, y, GENMAP_TOL));

  GenmapDestroyVector(x);
  GenmapDestroyVector(y);
}

void TestGenmapNormVector() {
  GenmapVector x, y;

  GenmapCreateVector(&x, 6);
  GenmapCreateVector(&y, 6);

  double dx[6] = {1., -1., 1., 1., 1., -1.};
  double dy[6] = {1., -1., 0., -2., 0., -1.};

  GenmapSetVector(x, dx);
  GenmapSetVector(y, dy);

  assert(GenmapNormVector(x, 1) == 6.0 && \
         fabs(GenmapNormVector(x, 2) - sqrt(6.0)) < GENMAP_TOL && \
         GenmapNormVector(x, -1) == 1 && \
         GenmapNormVector(y, 1) == 5.0 && \
         fabs(GenmapNormVector(y, 2) - sqrt(7.0)) < GENMAP_TOL && \
         GenmapNormVector(y, -1) == 2);

  GenmapDestroyVector(x);
  GenmapDestroyVector(y);
}

void TestGenmapScaleVector() {
  GenmapVector x, xscaled;

  GenmapCreateVector(&x, 4);
  GenmapCreateVector(&xscaled, 4);

  double dx[4] = {2.0, 4.0, 6.0, 8.0};
  double dxscaled[4] = {1.0, 2.0, 3.0, 4.0};

  GenmapSetVector(x, dx);
  GenmapSetVector(xscaled, dxscaled);

  GenmapScaleVector(xscaled, x, 0.5);

  assert(GenmapVectorsEqual(x, xscaled, GENMAP_TOL));

  GenmapDestroyVector(x);
  GenmapDestroyVector(xscaled);
}

void TestGenmapCreateOnesVector() {
  GenmapVector x, ones;

  GenmapCreateOnesVector(&x, 6);
  GenmapCreateVector(&ones, 6);

  double dones[6] = {1., 1., 1., 1., 1., 1.};
  GenmapSetVector(ones, dones);

  assert(GenmapVectorsEqual(x, ones, GENMAP_TOL));

  GenmapDestroyVector(x);
  GenmapDestroyVector(ones);
}
////------------------------------------------------------------------------------
//int32 test_3() {
//  Vector y;
//
//  zeros_vector(&y, 5);
//
//  double vans[5] = {0., 0., 0., 0., 0.};
//  Vector answer = { .size = 5, .vv = vans };
//
//  return (y.size == 5 && vectors_equal(&y, &answer, GENMAP_TOL));
//}
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//int32 test_5() {
//  Vector y;
//
//  random_vector(&y, 6, 0);
//
//  // TODO: add a test to check if inf_norm(y) <= 1.
//  return (y.size == 6);
//}
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//int32 test_7() {
//  double vx[6] = {1., -1., 1., 1., 1., -1.};
//  Vector x = { .size = 6, .vv = vx };
//
//  double vy[6] = {1., -1., 0., 2., 0., -1.};
//  Vector y = { .size = 6, .vv = vy };
//
//  mult_scalar_add_vector(&y, 1., &x, 1.);
//  double vans1[6] = {2., -2., 1., 3., 1., -2.};
//  Vector answer1 = { .size = 6, .vv = vans1 };
//
//  double vz[6] = {1., -1., 0., 2., 0., -1.};
//  Vector z = { .size = 6, .vv = vz };
//
//  mult_scalar_add_vector(&z, 1., &x, -1.);
//  double vans2[6] = {0., 0., -1., 1., -1., 0.};
//  Vector answer2 = { .size = 6, .vv = vans2 };
//
//  return vectors_equal(&answer1, &y, GENMAP_TOL) && \
//         vectors_equal(&answer2, &z, GENMAP_TOL);
//}
////------------------------------------------------------------------------------
//int32 test_8() {
//  double vx[6] = {1., -1., 1., 1., 1., -1.};
//  Vector x = { .size = 6, .vv = vx };
//
//  double vy[6] = {1., -1., 0., 2., 0., -1.};
//  Vector y = { .size = 6, .vv = vy };
//
//  return (dot_vector(&x, &y) == 5.);
//}
////------------------------------------------------------------------------------
int main() {
  TestGenmapCreateVector();
  TestGenmapSetVector();
  TestGenmapVectorsEqual();
  TestGenmapVectorsEqual();
  TestGenmapCopyVector();
  TestGenmapNormVector();
  TestGenmapCreateOnesVector();

  return 0;
}
////------------------------------------------------------------------------------
