#include "test.h"
#include "types.h"

#include <stdio.h>
//------------------------------------------------------------------------------
int test_1() {
  slong id;
  id = 1;

  return id;
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1,"gs_link1");

  return 0;
}
