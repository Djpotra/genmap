#ifndef _TQLI_H_
#define _TQLI_H_

inline double SIGN(double a, double b) {
  return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}

#endif
