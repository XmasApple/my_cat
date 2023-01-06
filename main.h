#ifndef SRC_CAT_MAIN_H_
#define SRC_CAT_MAIN_H_

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct s_flags {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
} s_flags;

void print_char(unsigned char c, s_flags flags, bool wtf);

void process_line(unsigned char *line, s_flags flags, bool wtf);

void process_file(char *filename, s_flags flags);

void read_opt(int argc, char *argv[], s_flags *flags);

int main(int argc, char *argv[]);

#endif  // SRC_CAT_MAIN_H_
