#ifndef _TEST_H_
#define _TEST_H_

#include "mpiwrapper.h"
#include "dtypes.h"

//------------------------------------------------------------------------------

void run_test(int32 (*test_fun)(), char *name);
void run_test_mpi(int32 (*test_fun)(), struct comm *c, char *name);

#endif
