#include "s21_cat.h"

int main(int argc, char *argv[]) {
  flag flags = parsing_flags(argc, argv);
  if (!flags.error) {
    file_handling(argc, argv, flags);
  } else {
    printf("usage: cat [-belnstuv] [file ...]");
  }
}
flag parsing_flags(int argc, char *argv[]) {
  flag flags = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int opt;
  struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                  {"number", no_argument, NULL, 'n'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "+benstEvT", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        flags.number_nonblank = 1;
        break;
      case 'e':
        flags.show_ends = 1;
        flags.suppress_v = 1;
        break;
      case 'n':
        flags.number_all = 1;
        break;
      case 's':
        flags.squeeze_blank = 1;
        break;
      case 't':
        flags.show_tabs = 1;
        flags.suppress_v = 1;
        break;
      case 'E':
        flags.show_ends_only = 1;
        break;
      case 'v':
        flags.suppress_v = 1;
        break;
      case 'T':
        flags.show_tabs_only = 1;
        break;
      case '?':
        flags.error = 1;
        break;
    }
  }
  return flags;
}
void file_handling(int argc, char *argv[], flag flags) {
  if (optind < argc) {
    for (int i = optind; i < argc; i++) {
      int index = 0;
      int eprint = 0;
      int preV = '\n';
      FILE *file;
      file = fopen(argv[i], "r");
      if (!file) {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      } else {
        int current = 0;
        while ((current = fgetc(file)) != EOF) {
          flag_operation_logic(flags, current, &preV, &index, &eprint);
        }
      }
    }
  }
}

void flag_operation_logic(flag flags, int current, int *preV, int *index,
                          int *eprint) {
  if (!(flags.squeeze_blank && *preV == '\n' && current == '\n' && *eprint)) {
    if (*preV == '\n' && current == '\n')
      *eprint = 1;
    else
      *eprint = 0;
    if (((flags.number_all && !flags.number_nonblank) ||
         (flags.number_nonblank && current != '\n')) &&
        *preV == '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }
    if ((flags.show_ends || flags.show_ends_only) && current == '\n') {
      if (flags.number_nonblank && *eprint) {
        printf("      \t$");
      } else {
        printf("$");
      }
    }
    if ((flags.show_tabs || flags.show_tabs_only) && current == '\t') {
      printf("^");
      current = '\t' + 64;
    }
    if (flags.suppress_v && current != '\n' && current != '\t') {
      if (current < 32) {
        printf("^");
        current += 64;
      } else if (current >= 127) {
        printf("^");
        current -= 64;
      }
    }
    printf("%c", current);
    *preV = current;
  }
}
