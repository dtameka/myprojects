#include "s21_grep.h"

int main(int argc, char **argv) {
    GREP_OPTIONS s21_grep_options = {0};
    FILE *fgrep = NULL;
    int grep_match_count = 0;
    int s21_grep_error = 0;
    int selected_file = 0;

    if (argc <= 2) {
        s21_grep_usage(argc);
        exit(2);
    }
// Step 1 - process s21_grep cli options
    s21_grep_init_options(argc, argv, &s21_grep_options);
// Step 2 - arrange a list of patterns and files for processing
// case when no -e or -f options are given ('hidden' -e)
    if ((s21_grep_options.e_option == 0 && s21_grep_options.f_option == 0))
        optind++;
    selected_file = optind;
// calculate number of file for processing
    s21_grep_options.number_of_files = argc - optind;
// if no option case, switch one stap back optind pointer
    int hidden_pattern = optind - 1;
// select pattern for processing - 'hidden -e' or with -e or/and -f option(s)
    if (s21_grep_options.regex_pattern_count == 0)
        strcpy(s21_grep_options.hidden_pattern, argv[hidden_pattern]);  // bare cli (no -ef) patten
// Step 3 - process all files that can be opened
    while (selected_file < argc) {
        fgrep = fopen(argv[selected_file], "rb");
        if (fgrep == NULL) {
            if (s21_grep_options.s_option == 0)
               fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[selected_file]);
            s21_grep_error = 2;
        } else {
            grep_match_count += s21_grep_process_files(fgrep, &s21_grep_options, argv, selected_file);
            fclose(fgrep);
        }  // process available files one by one
        selected_file++;  // go to the next file
    }
// define exit value for s21_grep depending on errors or found # of matches
    if (s21_grep_error != 2) {
        s21_grep_error = (grep_match_count  > 0) ? 0 : 1;
        }
        return s21_grep_error;
}  // EOF - main()

// main processing function
int s21_grep_process_files(FILE *fg, GREP_OPTIONS *s21_grep_options, char **argv, int selected_file) {
    char *file_line_buffer = NULL;
    size_t file_buffer_size;
    int match_line_count = 0;
    char match_line_buffer[BUFSIZ];
    regex_t regex;
    regmatch_t regmatch[2];
    int current_line = 0;
    size_t countreg = 0;
    size_t countnoreg = 0;
    int regex_compilation_result = 0;
    char *regex_pattern_check = (s21_grep_options->regex_pattern_count > 0) ? \
        s21_grep_options->regex_pattern : s21_grep_options->hidden_pattern;

// add -i option to regex pattern  and compile
    if (s21_grep_options->i_option)
        regex_compilation_result = regcomp(&regex, regex_pattern_check, REG_EXTENDED|REG_ICASE);
    else
        regex_compilation_result = regcomp(&regex, regex_pattern_check, REG_EXTENDED);
// exit s21_grep if regec compilation failed or start processing the file if regex compilation is successful
    if (regex_compilation_result != 0) {
       exit(regex_compilation_result);
    } else {
// read file(s) line by line and try to find matches
        while (getline(&file_line_buffer, &file_buffer_size, fg) != EOF) {
// execute regex compilation to find matches (and mismatches)
            int regexec_result = regexec(&regex, file_line_buffer, 2, regmatch, 0);
// supress EOL charracters in read strings
            int length = strlen(file_line_buffer);
            if (file_line_buffer[length - 1] == '\n' && length > 1) {
                file_line_buffer[length -1] = '\0';
            }
// increment scanned file lines - for option -n
            current_line++;
// main processing part for all matches and mismattechs
            if ((regexec_result == 0 && s21_grep_options->v_option == 0) || \
                (regexec_result == 1 && s21_grep_options->v_option == 1)) {
// increment count of matches and mismatches
                (regexec_result == 0) ? countreg++ : countnoreg++;
// no option processing
                if (!s21_grep_options->c_option && !s21_grep_options->n_option \
                    && !s21_grep_options->i_option && !s21_grep_options->o_option \
                    && !s21_grep_options->l_option) {
                    strcpy(match_line_buffer, file_line_buffer);
                    if (s21_grep_options->number_of_files > 1 && !s21_grep_options->h_option) {
                        printf("%s:", argv[selected_file]);
                    }
                    (length > 1) ? printf("%s\n", match_line_buffer) : printf("%s", match_line_buffer);
                }  // end of no option processing
// option -n processing
                if (s21_grep_options->n_option && !s21_grep_options->c_option && !s21_grep_options->o_option \
                    && !s21_grep_options->l_option) {
                    strcpy(match_line_buffer, file_line_buffer);
                    if (s21_grep_options->number_of_files > 1 && !s21_grep_options->h_option) {
                        printf("%s:", argv[selected_file]);
                    }
                    printf("%d:", current_line);
                    (length > 1) ? printf("%s\n", match_line_buffer) : printf("%s", match_line_buffer);
                }
// option -i processing
                if (s21_grep_options->i_option && !s21_grep_options->c_option && !s21_grep_options->n_option \
                    && s21_grep_options->o_option == 0 && !s21_grep_options->l_option) {
                    strcpy(match_line_buffer, file_line_buffer);
                    if (s21_grep_options->number_of_files > 1 && !s21_grep_options->h_option) {
                        printf("%s:", argv[selected_file]);
                    }
                    (length > 1) ? printf("%s\n", match_line_buffer) : printf("%s", match_line_buffer);
                }
// option -o processing
                if (s21_grep_options->o_option && !s21_grep_options->c_option && \
                    !s21_grep_options->l_option) {
                    if (s21_grep_options->o_option == 1 && s21_grep_options->v_option == 0) {
//  option -o processing Linux
#if defined (__linux__)
                        if (regmatch->rm_eo != regmatch->rm_so) {  // end != start - first match is here
                            char *regex_temp_buffer = file_line_buffer;
                            while (regexec_result == 0) {
                                if (s21_grep_options->number_of_files > 1 && !s21_grep_options->h_option) {
                                    printf("%s:", argv[selected_file]);
                                }
                                if (s21_grep_options->n_option == 1) {
                                    printf("%d:", current_line);
                                }
                                int offset = regmatch->rm_eo - regmatch->rm_so;  // end-start=match length
                                for (int k=0; k < offset; k++) {
                                    putchar((regex_temp_buffer + regmatch->rm_so)[k]);
                                }
                                putchar('\n');
                                regex_temp_buffer = regex_temp_buffer + regmatch->rm_eo;
                                regexec_result = regexec(&regex, regex_temp_buffer, 2, regmatch, 0);
                            }
                        }
#elif defined (__APPLE__)
                        int apple_line_match_count = 0;
                        if (regmatch->rm_eo != regmatch->rm_so) {  // end != start - first match is here
                            char *regex_temp_buffer = file_line_buffer;
                            int apple_file_match = 0;
                            int apple_line_match = 0;

                            apple_line_match_count = 0;
                            while (regexec_result == 0) {
                                if (s21_grep_options->number_of_files > 1 && !s21_grep_options->h_option \
                                    && apple_file_match == 0) {
                                    apple_file_match++;
                                    printf("%s:", argv[selected_file]);
                                }
                                if (s21_grep_options->n_option == 1 && apple_line_match == 0) {
                                    apple_line_match++;
                                    printf("%d:", current_line);
                                }
                                int offset = regmatch->rm_eo - regmatch->rm_so;  // end-start=match length
                                for (int k=0; k < offset; k++) {
                                    putchar((regex_temp_buffer + regmatch->rm_so)[k]);
                                }
                                putchar('\n');
                                regex_temp_buffer = regex_temp_buffer + regmatch->rm_eo;
                                regexec_result = regexec(&regex, regex_temp_buffer, 2, regmatch, 0);
                                apple_line_match_count++;
                            }
                        }
//  option -o processing Mac
#endif
                    }
                }
//  end -o option processing
            }  // if regexec_result == 0!
        }  // while (getline) cycle
// totals processing
        if (s21_grep_options->o_option && s21_grep_options->regex_newline_pattern == 1)
            countreg = current_line;
// option-l processing - match or mismatch then skip the rest!
        if (s21_grep_options->l_option && ((countreg && !s21_grep_options->v_option) || \
            (countnoreg && s21_grep_options->v_option))) {
            if (s21_grep_options->v_option && s21_grep_options->o_option && \
                s21_grep_options->regex_newline_pattern == 1) {  // do nothing
            } else {
                printf("%s\n", argv[selected_file]);
            }
        }
        match_line_count = (s21_grep_options->v_option)? countnoreg : countreg;
        match_line_count = (s21_grep_options->v_option && s21_grep_options->o_option && \
            s21_grep_options->regex_newline_pattern == 1)? 0 : match_line_count;
// option-c processing
        if (s21_grep_options->c_option && !s21_grep_options->l_option) {
            if (s21_grep_options->number_of_files > 1 && !s21_grep_options->l_option && \
                 !s21_grep_options->h_option) {
                printf("%s:", argv[selected_file]);
            }
            printf("%d\n", match_line_count);
        }
    }  // if regcomp == 0 big cycle!

    if (file_line_buffer != NULL)
        free(file_line_buffer);

    regfree(&regex);
    return (countreg > 0 ? countreg : countnoreg);
}  // EOF - grep-main processing!

// process s21_grep cli options with getopt
void s21_grep_init_options(int argument_count, char **argument_value, GREP_OPTIONS *take_options) {
    int opt = 0;
// get all s21_options with getopt + reorder options for processing ('grepping')
    while ((opt = getopt(argument_count, argument_value, ":ihvclnsoe:f:")) != -1) {  // : - for exceptions
        switch (opt) {
            case 'e':
                take_options->e_option++;
                s21_grep_regex_line_pattern(optarg, take_options);
                break;
            case 'i':
                take_options->i_option++;
                break;
            case 'v':
                take_options->v_option++;
                break;
            case 'c':
                take_options->c_option++;
                break;
            case 'l':
                take_options->l_option++;
                break;
            case 'n':
                take_options->n_option++;
                break;
            case 's':
                take_options->s_option++;
                break;
            case 'h':
                take_options->h_option++;
                break;
            case 'o':
                take_options->o_option++;
                break;
            case 'f':
                take_options->f_option++;
                s21_grep_regex_file_pattern(optarg, take_options);
                break;
            case '?':
                s21_grep_usage(argument_count);
                exit(1);
                break;
        }
    }
// close regex -e and -f patterns
    if (take_options->regex_pattern_count > 0 && strlen(take_options->regex_pattern) > 0)
        strcat(take_options->regex_pattern, ")");
// pocess new line in -f regex pattern
    if (take_options->regex_newline_pattern == 1 && take_options->o_option == 0)
        strcpy(take_options->regex_pattern, "[^n]");
}  // EOF - s21_grep_process_options

// get and append regex line from -e option
void s21_grep_regex_line_pattern(char *pattern, GREP_OPTIONS *take_options) {
    if (take_options->regex_pattern_count == 0) {
        strcat(take_options->regex_pattern, "(");
        strcat(take_options->regex_pattern, pattern);
    } else {
        strcat(take_options->regex_pattern, "|");
        strcat(take_options->regex_pattern, pattern);
    }
    take_options->regex_pattern_count++;
}  // EOF - s21_grep_regex_line_pattern

// get and append regex line from -f option
void s21_grep_regex_file_pattern(char *file_name, GREP_OPTIONS *take_options) {
  FILE *fpattern = NULL;
  char *file_buf = NULL;
  size_t file_buf_size;

  fpattern = fopen(file_name, "r");
  if (fpattern != NULL) {
      while (getline(&file_buf, &file_buf_size, fpattern) != EOF) {
          if (file_buf[0] == '\n') {
              take_options->regex_newline_pattern = 1;
              take_options->regex_pattern_count++;
          } else {
            int length = strlen(file_buf);
            if (file_buf[length - 1] == '\n') {
                file_buf[length - 1] = '\0';
            }
            s21_grep_regex_line_pattern(file_buf, take_options);  // create regex line with '|'
          }
        }
      fclose(fpattern);
      if (file_buf != NULL)
          free(file_buf);
  } else {
      if (take_options->s_option == 0)
      printf("%s:No such file or directory", file_name);
  }
}  // EOF - s21_grep_regex_file_pattern

// short output of s21_grep usage
void s21_grep_usage(int argument_count) {
#if defined (__linux__)
    if (argument_count <= 2) {
        fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
        fprintf(stderr, "Try 'grep --help' for more information.\n");
    } else {
        fprintf(stderr, "./s21_grep: invalid option -- '%c'\n", optopt);
        fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
        fprintf(stderr, "Try 'grep --help' for more information.\n");
    }
#elif defined (__APPLE__)
    if (argument_count < 2) {
        fprintf(stderr, "usage: s21_grep [-chilnosv]\n");
        fprintf(stderr, "[-e pattern] [-f file]\n");
        fprintf(stderr, "[file ...]\n");
    } else {
        fprintf(stderr, "./s21_grep: invalid option -- %c\n", optopt);  // Check for APPLE!
        fprintf(stderr, "usage: s21_grep [-chilnosv]\n");
        fprintf(stderr, "[-e pattern] [-f file]\n");
        fprintf(stderr, "[file ...]\n");
    }
#endif
}  // EOF - s21_cat_usage

