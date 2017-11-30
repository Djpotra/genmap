#ifndef _INV_POWER_H_
#define _INV_POWER_H_

#include "linalg.h"
#include "gswrapper.h"
#include "dtypes.h"

//------------------------------------------------------------------------------
void inv_power(Vector *eVector, Vector *alpha, Vector *beta, Vector *init,
                                                        int32 nc, int32 iter);

//------------------------------------------------------------------------------

#endif
