#include <math.h>
#include <assert.h>

#include "inv_power.h"

//------------------------------------------------------------------------------
void inv_power(Vector *eVector, Vector *alpha, Vector *beta, Vector *init,
                                                        int32 nc, int32 iter)
{
  assert(alpha->size == beta->size + 1);
  assert(alpha->size == eVector->size);

  int32 n = alpha->size;

  Vector x, y;
  create_vector(&x, n); create_vector(&y, n);
  random_vector(&x, n);

  // Remove the components of (1, 1, ...)
  double sum = 0.0;
  for (int32 i = 0; i < x.size; i++) {
    sum += x.vv[i];
  }

  sum /= sqrt(n);

  for (int i = 0; i < x.size; i++) {
    x.vv[i] -= sum;
  }

  if (n == 1) {
    eVector->vv[0] = alpha->vv[0];
    return;
  } else {
    for (int32 j = 0; j < iter; j++) {
      // y = Ax
      y.vv[0] = alpha->vv[0]*x.vv[0] + beta->vv[0]*x.vv[1];
      for (int32 i = 1; i < n - 1; i++) {
        y.vv[i] = beta->vv[i - 1]*x.vv[i - 1] + alpha->vv[i]*x.vv[i] +
                                                beta->vv[i + 1]*x.vv[i + 1];
      }

      // calculate infnty norm(y)
      scale_vector(&y, &y, 1./norm(&y, -1));

      copy_vector(&x, &y);
    }
  }

  copy_vector(eVector, &y);
}

//------------------------------------------------------------------------------
