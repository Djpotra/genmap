#include "linalg.h"
#include "test.h"

#include <math.h>
//------------------------------------------------------------------------------
int32 test_1() {
  Vector y;

  create_vector(&y, 10);

  return (y.size == 10);
}
//------------------------------------------------------------------------------
int32 test_2() {
  double vx[5] = {0., 0., 0., 0., 0.};
  Vector x = { .size = 5, .vv = vx };
  Vector y = { .size = 5, .vv = vx };

  double vz[5] = {0., 0., 0., 0., 1.};
  Vector z = { .size = 5, .vv = vz };

  return vectors_equal(&y, &x, 1e-12) && \
                           !vectors_equal(&y, &z, 1e-12);
}
//------------------------------------------------------------------------------
int32 test_3() {
  Vector y;

  zeros_vector(&y, 5);

  double vans[5] = {0., 0., 0., 0., 0.};
  Vector answer = { .size = 5, .vv = vans };

  return (y.size == 5 && vectors_equal(&y, &answer, 1e-12));
}
//------------------------------------------------------------------------------
int32 test_4() {
  Vector y;

  ones_vector(&y, 6);

  double vans[6] = {1., 1., 1., 1., 1., 1.};
  Vector answer = { .size = 6, .vv = vans };

  return (y.size == 6 && vectors_equal(&y, &answer, 1e-12));
}
//------------------------------------------------------------------------------
int32 test_5() {
  Vector y;

  random_vector(&y, 6);

  // TODO: add a test to check if inf_norm(y) <= 1.
  return (y.size == 6);
}
//------------------------------------------------------------------------------
int32 test_6() {
  double vx[6] = {1., -1., 1., 1., 1., -1.};
  Vector x = { .size = 6, .vv = vx };

  double vy[6] = {1., -1., 0., 2., 0., -1.};
  Vector y = { .size = 6, .vv = vy };

  return (norm_vector(&x, 1) == 6.0 && \
          norm_vector(&x, 2) == sqrt(6.0) && \
          norm_vector(&y, 1) == 5.0 && \
          norm_vector(&y, 2) == sqrt(7.0) );
}
//------------------------------------------------------------------------------
int32 test_7() {
  double vx[6] = {1., -1., 1., 1., 1., -1.};
  Vector x = { .size = 6, .vv = vx };

  double vy[6] = {1., -1., 0., 2., 0., -1.};
  Vector y = { .size = 6, .vv = vy };

  mult_scalar_add_vector(&y, 1., &x, 1.);
  double vans1[6] = {2., -2., 1., 3., 1., -2.};
  Vector answer1 = { .size = 6, .vv = vans1 };

  double vz[6] = {1., -1., 0., 2., 0., -1.};
  Vector z = { .size = 6, .vv = vz };

  mult_scalar_add_vector(&z, 1., &x, -1.);
  double vans2[6] = {0., 0., -1., 1., -1., 0.};
  Vector answer2 = { .size = 6, .vv = vans2 };

  return vectors_equal(&answer1, &y, 1e-12) && \
                        vectors_equal(&answer2, &z, 1e-12);
}
//------------------------------------------------------------------------------
int32 test_8() {
  double vx[6] = {1., -1., 1., 1., 1., -1.};
  Vector x = { .size = 6, .vv = vx };

  double vy[6] = {1., -1., 0., 2., 0., -1.};
  Vector y = { .size = 6, .vv = vy };

  return (dot_vector(&x, &y) == 5.);
}
//------------------------------------------------------------------------------
int32 main() {
  run_test(&test_1,"vector_1");
  run_test(&test_2,"vector_2");
  run_test(&test_3,"vector_3");
  run_test(&test_4,"vector_4");
  run_test(&test_5,"vector_5");
  run_test(&test_6,"vector_6");
  run_test(&test_7,"vector_7");
  run_test(&test_8,"vector_8");

  return 0;
}
//------------------------------------------------------------------------------
