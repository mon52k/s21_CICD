#define _GNU_SOURCE

#ifndef SIMPLE_BUSH_UTILS_GREP

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 10000

typedef struct {
  int regex_flag;
  int e;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char patterns[MAX_LINE_LENGTH];
  int error;
  int fileIndex;
  int patternIndex;
} GrepFlags;

GrepFlags ParseArguments(int argc, char *argv[], char *patterns, char **files);
void SearchFiles(GrepFlags flags, char *patterns, char **files);
void ProcessFile(FILE *file, GrepFlags flags, regex_t *preg, char *files);
void PrintMatches(regex_t *preg, char *line);
void PrintCount(GrepFlags flags, int c_counter, char *files, int lkey);
void PrintFileName(GrepFlags flags, char *files);
void PrintLineNumber(GrepFlags flags, int linenumber);
void SetPattern(GrepFlags *flags, char *patterns);
void ReadFile(GrepFlags *flags, char *patterns);
void ProcessFiles(GrepFlags *flags, char **files, int argc, char *argv[]);
void ProcessArguments(int argc, char *argv[], GrepFlags *flags, char *patterns,
                      char **files);

#endif
