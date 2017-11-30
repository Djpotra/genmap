#ifndef _EIG_H_
#define _EIG_H_

#include "linalg.h"
#include "gswrapper.h"
#include "dtypes.h"

//------------------------------------------------------------------------------
void power(Vector *eVector, Vector *alpha, Vector *beta, Vector *init,
                                                                   int32 iter);

//------------------------------------------------------------------------------

#endif
