#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

typedef struct {
int n;                 // -n - number lines
int b;                 // -b - number only non-empty lines
int s;                 // -s - suppress adjacent repetead blank lines
int e;                 // -display a '$' after the end of each line (\r\n combination will be shown as '^M$')
int t;                 // display a TAB chatracter as '^I'
int v;                 // display control characters (except LFD and TAB) using '^' notation
long count_lines;      // count lines - differently in Linux and Apple
char last_char;        // passing char element when processing several files
} CAT_OPTIONS;

void s21_cat_init_options(int argument_count, char **argument_value, CAT_OPTIONS *take_options);
void s21_cat_files(FILE *file_to_porcess, CAT_OPTIONS *take_options);
void s21_cat_usage(void);

#endif  //  SRC_CAT_S21_CAT_H_
