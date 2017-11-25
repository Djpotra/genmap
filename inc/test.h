#ifndef _TEST_H_
#define _TEST_H_

#include "mpiwrapper.h"

//------------------------------------------------------------------------------

void run_test(int (*test_fun)(), char *name);
void run_test_mpi(int (*test_fun)(), struct comm *c, char *name);

#endif
