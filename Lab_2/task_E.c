#include "array.h"
#include <stdio.h>

int main(){
  Array a = array_new(2);
  //printf(" StartPointer: %p \n", (void *)a.data);
  for(int i = 0; i < 1000; i++){
    array_insertBack(&a, i);
  }
  printf(" StartPointer: %p \n EndPointer: %p \n", (void *)a.data, (void *)a.data[a.back-1]);
  array_destroy(a);
  return 0;
}
