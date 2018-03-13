#include <genmap-impl.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

GenmapUInt32 genmap_srand_initialized = 0;
//------------------------------------------------------------------------------
// Vector operations
//
int GenmapCreateVector(GenmapVector *x, GenmapInt32 size) {
  /* Asserts:
       - size > 0
  */
  assert(size > 0);

  GenmapMalloc(1, x);
  if(*x == NULL) {
    printf("malloc failed in %s:%d", __FILE__, __LINE__);
    return 1;
  }

  (*x)->size = size;
  (*x)->data = NULL;

  GenmapMalloc(size, &(*x)->data);
  if((*x)->data == NULL) {
    printf("malloc failed in %s:%d", __FILE__, __LINE__);
    return 1;
  }

  return 0;
}

int GenmapVectorsEqual(GenmapVector x, GenmapVector y, GenmapScalar tol) {
  /* Asserts:
       - size of y == size of x
  */
  assert(x->size == y->size);

  GenmapInt32 n = x->size;
  for(GenmapInt32 i = 0; i < n; i++) {
    if(fabs(x->data[i] - y->data[i]) > tol) {
      return 0;
    }
  }

  return 1;
}

int GenmapSetVector(GenmapVector x, GenmapScalar *array) {
  memcpy(x->data, array, sizeof(GenmapScalar) * x->size);
  return 0;
}

int GenmapDestroyVector(GenmapVector x) {
  if(x->data) {
    free(x->data);
    x->data = NULL;
  }

  if(x) {
    free(x);
    x = NULL;
  }

  return 0;
}

int GenmapCopyVector(GenmapVector x, GenmapVector y) {
  /* Asserts:
       - size y = size x
  */
  assert(y->size == x->size);

  GenmapInt32 n = x->size;
  for(GenmapInt32 i = 0; i < n; i++) {
    x->data[i] = y->data[i];
  }

  return 0;
}

GenmapScalar GenmapNormVector(GenmapVector x, GenmapInt32 p) {
  assert(x->size > 0);

  GenmapInt32 n = x->size;
  GenmapScalar norm = 0;

  if(p == 1) {
    for(GenmapInt32 i = 0; i < n; i++) {
      norm += fabs(x->data[i]);
    }
  } else if(p == 2) {
    for(GenmapInt32 i = 0; i < n; i++) {
      norm += x->data[i] * x->data[i];
    }
    norm = sqrt(norm);
  } else if(p == -1) {
    norm = fabs(x->data[0]);

    for(GenmapInt32 i = 1; i < n; i++) {
      if(fabs(x->data[i]) > norm) norm = fabs(x->data[i]);
    }
  }

  return norm;
}

int GenmapScaleVector(GenmapVector y, GenmapVector x,  GenmapScalar alpha) {
  /* asserts:
       - size x = size y
  */
  assert(x->size == y->size);

  GenmapInt32 n = x->size;
  for(GenmapInt32 i = 0; i < n; i++) {
    y->data[i] = alpha * x->data[i];
  }
}
////------------------------------------------------------------------------------
//void random_vector(Vector *x, int32 size, int32 seed) {
//  create_vector(x, size);
//
//  if(!genmap_srand_initialized) {
//    srand(time(NULL) + seed);
//    genmap_srand_initialized = 1;
//  }
//
//  for(int32 i = 0; i < size; i++) {
//    x->data[i] = (double) rand() / RAND_MAX * 2. - 1.;
//  }
//}
////------------------------------------------------------------------------------
//void ones_vector(Vector *x, int32 size) {
//  create_vector(x, size);
//
//  for(int32 i = 0; i < size; i++) {
//    x->data[i] = 1.;
//  }
//}
////------------------------------------------------------------------------------
//void zeros_vector(Vector *x, int32 size) {
//  create_vector(x, size);
//
//  for(int32 i = 0; i < size; i++) {
//    x->data[i] = 0.;
//  }
//}
////------------------------------------------------------------------------------
//void mult_scalar_add_vector(Vector *y, double alpha, Vector *x, \
//                            double beta) {
//  /* Asserts:
//       - size y = size x
//  */
//  assert(y->size == x->size);
//
//  int32 n = x->size;
//  for(int32 i = 0; i < n; i++) {
//    y->data[i] = alpha * y->data[i] + beta * x->data[i];
//  }
//}
////------------------------------------------------------------------------------
//void z_axpby_vector(Vector *z, Vector *x, double alpha, \
//                    Vector *y, double beta) {
//  /* asserts:
//       - size z = size x = size y
//  */
//  assert(z->size == x->size);
//  assert(z->size == y->size);
//
//  int32 n = x->size;
//  for(int32 i = 0; i < n; i++) {
//    z->data[i] = alpha * x->data[i] + beta * y->data[i];
//  }
//}
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//double dot_vector(Vector *x, Vector *y) {
//  /* Asserts:
//       - size y = size x
//  */
//  assert(y->size == x->size);
//
//  double dot = 0.;
//
//  int32 n = x->size;
//  for(int32 i = 0; i < n; i++) {
//    dot += y->data[i] * x->data[i];
//  }
//
//  return dot;
//}
////------------------------------------------------------------------------------
//void print_vector(Vector *x) {
//  /* Asserts:
//       - size y > 0
//  */
//  printf("(%f", x->data[0]);
//  for(int32 i = 1; i < x->size - 1; i++) {
//    printf(", %f", x->data[i]);
//  }
//
//  if(x->size > 1) {
//    printf(", %f)", x->data[x->size - 1]);
//  } else {
//    printf(")");
//  }
//}
//
////------------------------------------------------------------------------------
//// Linear solves
//
//void symtridiag_solve(Vector *x, Vector *b, Vector *alpha, Vector *beta) {
//  assert(b->size == alpha->size);
//  assert(alpha->size == beta->size + 1);
//  assert(b->size > 0);
//
//  int32 n = b->size;
//
//  Vector diag;
//  create_vector(&diag, n); copy_vector(&diag, alpha);
//  create_vector(x, n); copy_vector(x, b);
//
//  for(int32 i = 0; i < n - 1; i++) {
//    double m = (beta->data[i] / diag.data[i]);
//    x->data[i + 1] = x->data[i + 1] - m * x->data[i];
//    diag.data[i + 1] = diag.data[i + 1] - m * beta->data[i];
//  }
//
//  x->data[n - 1] = x->data[n - 1] / diag.data[n - 1];
//  for(int32 i = n - 2; i >= 0; i--) {
//    x->data[i] = (x->data[i] - beta->data[i] * x->data[i + 1]) / diag.data[i];
//  }
//
//  return;
//}
//
////------------------------------------------------------------------------------
