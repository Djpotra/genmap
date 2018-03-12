#ifndef _TEST_H_
#define _TEST_H_

#include <stdarg.h>

#include "mpiwrapper.h"
#include "dtypes.h"
//------------------------------------------------------------------------------
// Test functions
void run_test(int32(*test_fun)(), char *name);
void run_test_mpi(int32(*test_fun)(), struct comm *c, char *name);

//------------------------------------------------------------------------------
// Debug utilities
void debugi(int32 nparams, ...);
void debugl(int32 nparams, ...);
void debugd(int32 nparams, ...);

#endif
