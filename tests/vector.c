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
  Vector y;

  zeros_vector(&y, 5);

  double vans[5] = {0., 0., 0., 0., 0.};
  Vector answer = { .size = 5, .vv = vans };

  return (y.size == 5 && vectors_equal(&y, &answer, 1e-12));
}
//------------------------------------------------------------------------------
int test_3() {
  Vector y;

  ones_vector(&y, 6);

  double vans[6] = {1., 1., 1., 1., 1., 1.};
  Vector answer = { .size = 6, .vv = vans };

  return (y.size == 6 && vectors_equal(&y, &answer, 1e-12));
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1,"vector_1");
  run_test(&test_2,"vector_2");
  run_test(&test_3,"vector_3");

  return 0;
}
//------------------------------------------------------------------------------
