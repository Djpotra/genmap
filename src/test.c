#include <stdio.h>

#include "test.h"

//------------------------------------------------------------------------------
void run_test(int32 (*test_fun)(), char *name) {
  int32 passed = test_fun();

  if (passed) {
    printf("Test %s: Passed.\n", name);
  } else {
    printf("Test %s: Failed.\n", name);
  }
}

void run_test_mpi(int32 (*test_fun)(), struct comm *c, char *name) {
  int32 passed = test_fun(c);

  if (passed) {
    printf("Test %s: Passed.\n", name);
  } else {
    printf("Test %s: Failed.\n", name);
  }
}

//------------------------------------------------------------------------------
// Debug utilities
void debugi(int32 nparams, ...)
{
  va_list valist;

  va_start(valist, nparams);

  for (int32 i = 0; i < nparams; i++) {
    printf("param %d: %d\n", i, va_arg(valist, int));
  }
}

void debugl(int32 nparams, ...)
{
  va_list valist;

  va_start(valist, nparams);

  for (int32 i = 0; i < nparams; i++) {
    printf("param %d: %ld\n", i, va_arg(valist, long));
  }
}

void debugd(int32 nparams, ...)
{
  va_list valist;

  va_start(valist, nparams);

  for (int32 i = 0; i < nparams; i++) {
    printf("param %d: %lf\n", i, va_arg(valist, double));
  }
}

//------------------------------------------------------------------------------
