#include <stdio.h>
#include "lib.h"
int main(int argc, char** argv) {
  helper_args_t helper_args;
  helper_args.string = argv[0];
  helper_args.target = '/';
  char* result = helper_func(&helper_args);
  printf("%s\n", result);
  return sizeof(helper_args_t);
}