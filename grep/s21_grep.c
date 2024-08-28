
#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char *search_patterns = (char *)malloc(256 * sizeof(char));
  if (search_patterns == NULL) {
    printf("memory for the array cannot be allocated");
  } else {
    strcpy(search_patterns, "");
  }

  int num_files = 10;
  char **file_names = malloc(num_files * sizeof(char *));
  if (file_names == NULL) {
    printf("memory for the array cannot be allocated");
  }
  GrepFlags grep_flags =
      ParseArguments(argc, argv, search_patterns, file_names);
  if (grep_flags.error == false) {
    SearchFiles(grep_flags, search_patterns, file_names);
  }
  free(search_patterns);
  for (int i = 0; i < grep_flags.fileIndex; i++) {
    free(file_names[i]);
  }
  free(file_names);
  return 0;
}

void SearchFiles(GrepFlags flags, char *patterns, char **files) {
  regex_t preg;
  int reti = regcomp(&preg, patterns, flags.regex_flag);
  if (reti) {
    fprintf(stderr, "Can't compile regular expression\n");
  }
  for (int i = 0; i < flags.fileIndex; i++) {
    FILE *file = fopen(files[i], "r");
    if (file == NULL) {
      if (flags.s == false) {
        fprintf(stderr, "grep: %s: No such file or directory\n", files[i]);
      }
      continue;
    } else {
      ProcessFile(file, flags, &preg, files[i]);
    }
    fclose(file);
  }
  regfree(&preg);
}

void ProcessFile(FILE *file, GrepFlags flags, regex_t *preg, char *files) {
  int c_counter = 0;
  int linenumber = 1;
  int lkey = 0;
  char line[MAX_LINE_LENGTH];
  regmatch_t match;
  while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    line[strcspn(line, "\n")] = 0;
    if (regexec(preg, line, 1, &match, 0) == 0 && flags.v == false) {
      c_counter++;
      if (flags.c == false && flags.l == false) {
        PrintFileName(flags, files);
        PrintLineNumber(flags, linenumber);
        if (flags.o == false) {
          printf("%s\n", line);
        } else {
          PrintMatches(preg, line);
        }
      }
      lkey = 1;
    } else if (regexec(preg, line, 1, &match, 0) != 0 && flags.v == true) {
      c_counter++;
      if (flags.c == false && flags.l == false) {
        PrintFileName(flags, files);
        PrintLineNumber(flags, linenumber);
        printf("%s\n", line);
      }
      lkey = 1;
    }

    linenumber++;
  }

  PrintCount(flags, c_counter, files, lkey);
}

void PrintMatches(regex_t *preg, char *line) {
  int offset = 0;
  regmatch_t match;
  while (regexec(preg, line + offset, 1, &match, 0) == 0) {
    printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
           line + offset + match.rm_so);
    offset += match.rm_eo;
    if (match.rm_so == -1) {
      break;
    }
  }
}

void PrintCount(GrepFlags flags, int c_counter, char *files, int lkey) {
  if (flags.c == true && flags.l == false) {
    PrintFileName(flags, files);
    printf("%d\n", c_counter);
  } else if (flags.c == true && flags.l == true) {
    PrintFileName(flags, files);
    if (lkey == 1) {
      printf("1\n");
    } else {
      printf("0\n");
    }
  }
  if (flags.l == true && lkey == 1) {
    printf("%s\n", files);
  }
}

void PrintFileName(GrepFlags flags, char *files) {
  if (flags.fileIndex > 1 && flags.h == false) {
    printf("%s:", files);
  }
}

void PrintLineNumber(GrepFlags flags, int linenumber) {
  if (flags.n == true) {
    printf("%d:", linenumber);
  }
}
GrepFlags ParseArguments(int argc, char *argv[], char *patterns, char **files) {
  GrepFlags flags = {.error = false,
                     .regex_flag = false,
                     .e = false,
                     .v = false,
                     .c = false,
                     .l = false,
                     .n = false,
                     .h = false,
                     .s = false,
                     .f = false,
                     .o = false,
                     .fileIndex = false,
                     .patternIndex = false};

  int currentFlag;
  while ((currentFlag = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) !=
         -1) {
    switch (currentFlag) {
      case 'e':
        SetPattern(&flags, patterns);
        break;
      case 'i':
        flags.regex_flag |= REG_ICASE;
        break;
      case 'v':
        flags.v = true;
        break;
      case 'c':
        flags.c = true;
        break;
      case 'l':
        flags.l = true;
        break;
      case 'n':
        flags.n = true;
        break;
      case 'h':
        flags.h = true;
        break;
      case 's':
        flags.s = true;
        break;
      case 'f':
        ReadFile(&flags, patterns);
        break;
      case 'o':
        flags.o = true;
        break;
      case '?':
        flags.error = true;
        break;
      default:
        break;
    }
  }
  ProcessFiles(&flags, files, argc, argv);
  ProcessArguments(argc, argv, &flags, patterns, files);
  return flags;
}

void ProcessFiles(GrepFlags *flags, char **files, int argc, char *argv[]) {
  if ((*flags).e || (*flags).f) {
    (*flags).fileIndex = argc - optind;
    if ((*flags).fileIndex > 0) {
      for (int i = 0; i < (*flags).fileIndex; i++) {
        files[i] = strdup(argv[optind + i]);
      }
    }
  }
}

void ProcessArguments(int argc, char *argv[], GrepFlags *flags, char *patterns,
                      char **files) {
  if ((*flags).e == false && (*flags).f == false) {
    bool patternSet = false;
    for (int i = 1; i < argc; i++) {
      files[(*flags).fileIndex] = NULL;
      if (argv[i][0] == '-') {
        continue;
      }
      if (patternSet == false) {
        strcat(patterns, argv[i]);
        patternSet = true;
      } else {
        files[(*flags).fileIndex] = strdup(argv[i]);
        (*flags).fileIndex++;
      }
    }
  }
}

void SetPattern(GrepFlags *flags, char *patterns) {
  (*flags).e = true;
  (*flags).regex_flag |= REG_EXTENDED;
  (*flags).patternIndex++;
  if ((*flags).patternIndex != 1) {
    strcat(patterns, "|");
  }
  strcat(patterns, optarg);
}

void ReadFile(GrepFlags *flags, char *patterns) {
  int flagerr = 0;
  (*flags).f = true;
  (*flags).regex_flag |= REG_EXTENDED;
  FILE *file = fopen(optarg, "r");
  if (file == NULL) {
    printf("%s: No such file or directory\n", optarg);
    flagerr = 1;
  } else if (!flagerr) {
    char *line = NULL;
    size_t buffer_size = 100;
    line = (char *)malloc(buffer_size * sizeof(char));
    if (line == NULL) {
      fprintf(stderr, "Error! Memory allocation error!\n");
      flagerr = 1;
    } else if (!flagerr) {
      while (fgets(line, buffer_size, file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        (*flags).patternIndex++;
        if ((*flags).patternIndex != 1) {
          strcat(patterns, "|");
        }
        strcat(patterns, line);
        if (line[strlen(line) - 1] != '\n' && !flagerr) {
          buffer_size *= 2;
          char *temp = realloc(line, buffer_size);
          if (temp == NULL) {
            fprintf(stderr, "Error! Memory allocation error!\n");
            free(line);
            flagerr = 1;
          }
          line = temp;
        }
      }
      fclose(file);
      free(line);
    }
  }
}
