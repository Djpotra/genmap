#include <genmap-impl.h>

#include <stdio.h>
//------------------------------------------------------------------------------
void test_1() {
  GenmapInt32 n = 3;
  GenmapVector alpha, beta, eVector, init;

  GenmapCreateOnesVector(&alpha, n);
  GenmapCreateOnesVector(&init, n);
  GenmapCreateVector(&eVector, n);

  GenmapCreateVector(&beta, n - 1); beta->data[0] = 2.0; beta->data[1] = 3.0;

  GenmapPowerIter(eVector, alpha, beta, init, 15);

  printf("(%lf, %lf, %lf)\n", eVector->data[0], eVector->data[1],
         eVector->data[2]);

  GenmapDestroyVector(alpha);
  GenmapDestroyVector(beta);
  GenmapDestroyVector(eVector);
  GenmapDestroyVector(init);
}

//------------------------------------------------------------------------------
void test_2() {
  GenmapInt32 n = 4;
  GenmapVector alpha, beta, eVector, init;

  GenmapCreateVector(&alpha, n);
  for(GenmapInt32 i = 0; i < n; i++) alpha->data[i] = (double) i + 2.0;
  GenmapCreateVector(&beta, n - 1); GenmapCreateOnesVector(&beta, n - 1);

  GenmapCreateVector(&init, n); GenmapCreateOnesVector(&init, n);
  GenmapCreateVector(&eVector, n);

  GenmapPowerIter(eVector, alpha, beta, init, 15);

  printf("(%lf, %lf, %lf, %lf)\n",
         eVector->data[0], eVector->data[1], eVector->data[2], eVector->data[3]);

  GenmapDestroyVector(alpha);
  GenmapDestroyVector(beta);
  GenmapDestroyVector(eVector);
  GenmapDestroyVector(init);
}

//------------------------------------------------------------------------------
void test_3() {
  GenmapInt32 n = 3;
  GenmapVector alpha, beta, eVector, init;

  GenmapCreateVector(&alpha, n); GenmapCreateOnesVector(&alpha, n);
  GenmapCreateVector(&beta, n - 1); beta->data[0] = 2.0; beta->data[1] = 3.0;

  GenmapCreateVector(&init, n); GenmapCreateOnesVector(&init, n);
  GenmapCreateVector(&eVector, n);

//  GenmapInvPowerIter(eVector, alpha, beta, init, 15);
//
//  printf("(%lf, %lf, %lf)\n", eVector->data[0], eVector->data[1],
//         eVector->data[2]);

  GenmapDestroyVector(alpha);
  GenmapDestroyVector(beta);
  GenmapDestroyVector(eVector);
  GenmapDestroyVector(init);
}

//------------------------------------------------------------------------------
void test_4() {
  GenmapInt32 n = 4;
  GenmapVector alpha, beta, eVector, init;

  GenmapCreateOnesVector(&alpha, n);
  for(GenmapInt32 i = 0; i < n; i++) alpha->data[i] = (double) i + 2.0;
  GenmapCreateVector(&beta, n - 1); GenmapCreateOnesVector(&beta, n - 1);

  GenmapCreateOnesVector(&init, n);
  GenmapCreateVector(&eVector, n);

//  GenmapInvPowerIter(eVector, alpha, beta, init, 15);
//
//  printf("(%lf, %lf, %lf, %lf)\n",
//         eVector->data[0], eVector->data[1], eVector->data[2], eVector->data[3]);

  GenmapDestroyVector(alpha);
  GenmapDestroyVector(beta);
  GenmapDestroyVector(eVector);
  GenmapDestroyVector(init);
}

//------------------------------------------------------------------------------
int main() {
  test_1();
  test_2();
  test_3();
  test_4();

  return 0;
}

//------------------------------------------------------------------------------
