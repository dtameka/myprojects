#include "s21_cat.h"

int main(int argc, char **argv) {
    int i = 1;
    CAT_OPTIONS s21_cat_options = {};
    s21_cat_options.last_char = '\n';
    FILE *fcat = NULL;
    int s21_cat_error = 0;
    // Step 1 - init options
    s21_cat_init_options(argc, argv, &s21_cat_options);
    // Step 2 - select files to cat
#if defined (__linux__)
     s21_cat_options.count_lines = 0;  // for Linux one count trough all files is calculating
#endif
    for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {  // skip all s21_cat options, filter out only files
        continue;
      } else {
          fcat = fopen(argv[i], "r");
          if (fcat == NULL) {
              fprintf(stderr, "%s: %s: %s\n", argv[0], argv[i], strerror(errno));  // check it for Linux
              s21_cat_error++;
          } else {
              // Step 3 - process files
              s21_cat_files(fcat, &s21_cat_options);
              fclose(fcat);
            }
        }
    }
    return s21_cat_error;
}  // EOF - main

void s21_cat_init_options(int argument_count, char **argument_value, CAT_OPTIONS *take_options) {
    int next_option = 0;
    static struct option long_options[] = {
#if defined (__APPLE__)
        { "", no_argument, NULL, 'b' },
        { "", no_argument, NULL, 'e' },  // equivalent -vE
        { "", no_argument, NULL, 'n' },
        { "", no_argument, NULL, 's' },
        { "", no_argument, NULL, 't' },  // equivalent -vT
        { "", no_argument, NULL, 'v' },
#elif defined (__linux__)
        { "number-nonblank",  no_argument, NULL, 'b' },
        { "",                 no_argument, NULL, 'e' },  // equivalent -vE
        { "number",           no_argument, NULL, 'n' },
        { "squeeze-blank",    no_argument, NULL, 's' },
        { "",                 no_argument, NULL, 't' },  // equivalent -vT
        { "show-nonprinting", no_argument, NULL, 'v' },
        { "show-ends",        no_argument, NULL, 'E' },  // equivalent to -vE
        { "show-tabs",        no_argument, NULL, 'T' },  // equivalent to -vT
        { "show-all",         no_argument, NULL, 'A' },  // equivalent to -vET
#endif
        { NULL,                         0, NULL,  0  },
    };
    do {
#if defined (__linux__)
      next_option = getopt_long(argument_count, argument_value, ":benstuvATE", long_options, NULL);
#elif defined (__APPLE__)
      next_option = getopt_long(argument_count, argument_value, ":benstuv", long_options, NULL);
#endif
      switch (next_option) {
          case 'b':
              take_options->n = 1;
              take_options->b = 1;
              break;
          case 'e':
              take_options->e = 1;
              take_options->v = 1;
              break;
          case 'n':
              take_options->n = 1;
              break;
          case 's':
              take_options->s = 1;
              break;
          case 't':
              take_options->t = 1;
              take_options->v = 1;
              break;
          case 'u':   // ignored in both Linux and Apple
              break;
          case 'v':
              take_options->v = 1;
              break;
#if defined (__linux__)
          case 'A':
              take_options->e = 1;
              take_options->t = 1;
              take_options->v = 1;
              break;
          case 'E':
              take_options->e = 1;
              break;
          case 'T':
              take_options->t = 1;
              break;
#endif
          case '?':
              s21_cat_usage();
              exit(1);
              break;
      }
    } while (next_option != -1);
}  // EOF - s21_cat_process_options

void s21_cat_files(FILE *file_to_porcess, CAT_OPTIONS *take_options) {
    int ch;               //  main selector - 'current char' (from fgetc)
    char prev;            //  main selector - 'next char' (from fgets)
    int duplicate_line;   //  duplication line

#if defined (__APPLE__)
    take_options->count_lines = 0;   //  new line counter
#endif
    duplicate_line = 0;
    prev = take_options->last_char;
    while ((ch = fgetc(file_to_porcess)) != EOF) {
        if (prev == '\n') {
            if (take_options->s) {
                if (ch == '\n') {
                    if (duplicate_line) {
                      continue;
                    }
                    duplicate_line = 1;
                } else {
                    duplicate_line = 0;
                }
            }  // sqeeze empty lines
            if (take_options->n && (!take_options->b || (ch != '\n'))) {
                  printf("%6ld\t", ++take_options->count_lines);
              }  // number lines
          }
      if (ch == '\n') {
          if (take_options->e)
              putchar('$');  // print line ends
      } else if (ch == '\t') {
          if (take_options->t) {
              putchar('^');
              putchar('I');  // print tabs
              prev = ch;
              continue;
          }
      } else if (take_options->v) {
          if (ch >= 0 && ch < 32 && ch != 9 && ch != 10 && ch != 12 && ch != 127) {
              putchar('^');
              ch = ch + 64;
          }
          if (ch >= 128 && ch <= 159) {
              printf("M-^");
              ch = ch - 128 + 64;
          }
          if (ch == 127) {
              printf("^?");
              prev = ch;
              continue;
          }
#if defined (__linux__)
          if (ch >= 160 && ch <= 254) {
              printf("M-");
              ch = ch + 128;
          }
          if (ch == 0xff) {
              printf("M-^?");
              prev = ch;
              continue;
          }
#endif
      }  // for new lines
      prev = ch;  // swap previous char with current
      take_options->last_char = prev;
      putchar(ch);
    }
}

void s21_cat_usage(void) {
#if defined (__linux__)
    fprintf(stderr, "./s21_cat: invalid option\n");
    fprintf(stderr, "Try 'cat --help' for more information.\n");
#elif defined (__APPLE__)
    fprintf(stderr, "./s21_cat: illegal option\n");
    fprintf(stderr, "usage: ./s21_cat [-benstuv] [file ...]\n");
#endif
}
