#ifndef SIMPLE_BUSH_UTILS_CAT

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int number_all;
  int number_nonblank;
  int squeeze_blank;
  int show_ends;
  int show_tabs;
  int suppress_v;
  int show_ends_only;
  int show_tabs_only;
  int error;
} flag;

flag parsing_flags(int argc, char *argv[]);
void flag_operation_logic(flag flags, int c, int *preV, int *index,
                          int *eprint);
void file_handling(int argc, char *argv[], flag flags);

#endif
