#include "main.h"

static struct option const long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {"show-ends", no_argument, NULL, 'E'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"show-all", no_argument, NULL, 'A'},
    {NULL, 0, NULL, 0}};
static char const short_options[] = "benstvET";

void print_char(unsigned char c, s_flags flags, bool wtf) {
  if (flags.v) {
    if (c >= 32) {
      if (c == 127) {
        putchar('^');
        putchar('?');
      } else if (c > 127) {
        putchar('M');
        putchar('-');
        if (c >= 128 + 32) {
          if (c < 128 + 127) {
            putchar(c - 128);
          } else {
            putchar('^');
            putchar('?');
          }
        } else {
          putchar('^');
          putchar(c - 128 + 64);
        }
      }
    } else if (c == '\t' && !flags.t) {
    } else if (c == '\n') {
      if (flags.e) putchar('$');
    } else {
      c += 64;
      putchar('^');
    }
  } else {
    if (c == '\t' && flags.t) {
      c += 64;
      putchar('^');
    } else if (c == '\n') {
      if (flags.e) putchar('$');
    }
  }
  if (c < 127 || (wtf && !flags.v)) putchar(c);
}

void process_line(unsigned char *line, s_flags flags, bool wtf) {
  for (char c = *line; c != '\0'; c = *++line) {
    print_char(c, flags, wtf);
  }
}

void process_file(char *filename, s_flags flags) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", filename);
  } else {
    unsigned char *line = NULL;
    bool prev_blank = false;
    bool is_blank = false;
    size_t len = 0;
    ssize_t read;
    int line_number = 1;
    while ((read = getline((char **)&line, &len, file)) != -1) {
      is_blank = line[0] == '\n';
      if (is_blank && flags.s && prev_blank) {
        continue;
      }
      if (flags.b) {
        if (!is_blank) printf("%6d\t", line_number++);
      } else if (flags.n) {
        printf("%6d\t", line_number++);
      }
      bool wtf = flags.n | flags.b | flags.e | flags.t | flags.s;
      process_line(line, flags, wtf);
      prev_blank = is_blank;
    }
    free(line);
    fclose(file);
  }
}

void read_opt(int argc, char *argv[], s_flags *flags) {
  int opt;
  while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        flags->n = flags->b = true;
        break;
      case 'e':
        flags->e = flags->v = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 't':
        flags->t = flags->v = true;
        break;
      case 'v':
        flags->v = true;
        break;
      case 'E':
        flags->e = true;
        break;
      case 'T':
        flags->t = true;
        break;
      default:
        fprintf(stderr, "Usage: %s [-%s] [file...]\n", argv[0], short_options);
        exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char *argv[]) {
  s_flags flags = {0};
  read_opt(argc, argv, &flags);

  int i = optind;
  for (; i < argc; i++) {
    char *fname = argv[i];
    process_file(fname, flags);
  }
  return 0;
}
