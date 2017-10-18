#include "tqli.h"

#include <math.h>
//------------------------------------------------------------------------------
double pythag(double a, double b) {
  static double dsqrarg;
  #define DSQR(a) ((dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg)
  double absa, absb;
  absa = fabs(a);
  absb = fabs(b);

  if (absa > absb)
    return absa * sqrt(1.0 + DSQR(absb / absa));
  else
    return (absb == 0.0 ? 0.0 : absb * sqrt(1.0 + DSQR(absa / absb)));
  #undef DSQR
}
//------------------------------------------------------------------------------
void tqli(double *d, double *e, int n, double **z) {
  int m, l, iter, i, k;
  double s, r, p, g, f, dd, c, b;

  // In the algorithm, e[1] i.e., e[0] is arbitray
  // but we send actual value in there
  e[n - 1] = 0.0; // so e is a n vector

  for (l = 0; l < n; l++) {
    iter = 0;
    do {
      // Look for a small subdiagonal entry to split the matrix
      for (m = l; m < n - 1; m++) {
        dd = fabs(d[m]) + fabs(d[m + 1]);
        if (fabs(e[m]) + dd == dd) {
           break;
        }
      }

      if (m != l) {
        g = (d[l + 1] - d[l])/(2.0*e[l]);
        r = pythag(g, 1.0);
        g = d[m] - d[l] + e[l]/(g + SIGN(r, g));
        s = c = 1.0;
        p = 0.0;

        for (i = m - 2; i >= l; i--) {
          f = s*e[i];
          b = c*e[i];
  
          e[i + 1] = (r = pythag(f, g));
          if (r == 0.0) {
            d[i + 1] -= p;
            e[m] = 0.0;
            break;
          }
  
          s = f/r;
          c = g/r;
          g = d[i + 1] - p;
          r = (d[i] - g)*s + 2.0*c*b;
          d[i + 1] = g + (p = s*r);
          g = c*r - b;
  
          // Loop for eigenvactors here, omitted for now
        }
  
        if (r == 0.0 && i >= l) continue;
        d[l] -= p;
        e[l] = g;
        e[m] = 0.0;
      }
    } while (m != l && (m < n - 1));
  }
}
//------------------------------------------------------------------------------
