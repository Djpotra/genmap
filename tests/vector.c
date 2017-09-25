#include "linalg.h"
#include "test.h"
#include <stdio.h>
//------------------------------------------------------------------------------
int test_1() {
  Vector y;

  create_vector(&y, 10);

  return (y.size == 10);
}
//------------------------------------------------------------------------------
int test_2() {
  double v[5] = {0., 0., 0., 0., 0.};

  Vector x = { .size = 5, .vv = v };
  Vector y = { .size = 5, .vv = v };

  return vectors_equal(&y, &x, 1e-12);
}
//------------------------------------------------------------------------------
int test_3() {
  Vector y;

  zeros_vector(&y, 5);

  double vans[5] = {0., 0., 0., 0., 0.};
  Vector answer = { .size = 5, .vv = vans };

  return (y.size == 5 && vectors_equal(&y, &answer, 1e-12));
}
//------------------------------------------------------------------------------
int test_4() {
  Vector y;

  ones_vector(&y, 6);

  double vans[6] = {1., 1., 1., 1., 1., 1.};
  Vector answer = { .size = 6, .vv = vans };

  return (y.size == 6 && vectors_equal(&y, &answer, 1e-12));
}
//------------------------------------------------------------------------------
int test_5() {
  Vector y;

  random_vector(&y, 6);

  // TODO: add a test to check if inf_norm(y) <= 1.
  return (y.size == 6);
}
//------------------------------------------------------------------------------
int test_6() {
  double v1[6] = {1., -1., 1., 1., 1., -1.};
  Vector x = { .size = 6, .vv = v1 };

  double v2[6] = {1., -1., 0., 2., 0., -1.};
  Vector y = { .size = 6, .vv = v2 };

  return (norm_vector(&x, 1) == 6.0 && \
          norm_vector(&x, 2) == 6.0 && \
          norm_vector(&y, 1) == 5.0 && \
          norm_vector(&y, 2) == 7.0 );
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1,"vector_1");
  run_test(&test_2,"vector_2");
  run_test(&test_3,"vector_3");
  run_test(&test_4,"vector_4");
  run_test(&test_5,"vector_5");
  run_test(&test_6,"vector_6");

  return 0;
}
//------------------------------------------------------------------------------
