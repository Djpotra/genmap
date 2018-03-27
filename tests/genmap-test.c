#include <genmap.h>

#include <stdio.h>
//
// Test GenmapInit, GenmpaFinalize, etc.
int main(int argc, char **argv) {
  GenmapHandle h;
  GenmapInit(&h, argc, argv);
  GenmapFinalize(h);
}
