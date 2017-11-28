#include <stdio.h>

#include "test.h"

//------------------------------------------------------------------------------
void run_test(int (*test_fun)(), char *name) {
  int passed = test_fun();

  if (passed) {
    printf("Test %s: Passed.\n", name);
  } else {
    printf("Test %s: Failed.\n", name);
  }
}

void run_test_mpi(int (*test_fun)(), struct comm *c, char *name) {
  int passed = test_fun(c);

  if (passed) {
    printf("Test %s: Passed.\n", name);
  } else {
    printf("Test %s: Failed.\n", name);
  }
}

//------------------------------------------------------------------------------
