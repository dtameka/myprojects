#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h>
#include <getopt.h>


// main s21_grep struc for keeping option processing results and key information
typedef struct {
int e_option;
int h_option;
int i_option;
int v_option;
int l_option;
int n_option;
int c_option;
int s_option;
int o_option;
int f_option;
char hidden_pattern[4096];
char regex_pattern[4096];
int regex_pattern_count;
int regex_newline_pattern;
int number_of_files;
} GREP_OPTIONS;

// function to process cli options and fill the main struct
void s21_grep_init_options(int argument_count, char **argument_value, GREP_OPTIONS *take_options);

// function to turn cli input into regex cimpatible line
void s21_grep_regex_line_pattern(char *pattern, GREP_OPTIONS *get_grep_options);

// function to turn file input into regex cimpatible line
void s21_grep_regex_file_pattern(char *, GREP_OPTIONS *get_grep_options);

// main processing and output function
int s21_grep_process_files(FILE *fp, GREP_OPTIONS *s21_grep_options, char **argv, int current_file);

// function to provide basic s21_grep use information
void s21_grep_usage(int argument_count);

#endif  // SRC_GREP_S21_GREP_H_
