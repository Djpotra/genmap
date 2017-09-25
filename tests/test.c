#include <stdio.h>

#include "test.h"

//------------------------------------------------------------------------------
void run_test(int (*test_fun)(), char *name) {
  int passed = test_fun();

  if (passed) {
    printf("Test %.8s: Passed.\n", name);
  } else {
    printf("Test %.8s: Failed.\n", name);
  }
}
//------------------------------------------------------------------------------