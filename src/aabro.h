
//
// Copyright (c) 2013-2014, John Mettraux, jmettraux+flon@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Made in Japan.
//

#ifndef AABRO_H
#define AABRO_H

#include <regex.h>

//
// abr_parser

typedef struct abr_parser {
  char *name;
  short type;
  char *string;
  size_t string_length;
  regex_t *regex;
  int min; int max;
  struct abr_parser **children;
} abr_parser;

void abr_parser_free(abr_parser *p);

char *abr_parser_to_string(abr_parser *p);

//
// abr_tree

typedef struct abr_tree {
  char *name;
  short result; // -1 error, 0 failure, 1 success
  size_t offset;
  size_t length;
  char *note; // set in case of error
  abr_parser *parser;
  struct abr_tree **children;
} abr_tree;

void abr_tree_free(abr_tree *t);

/*
 * Returns a string representation (JSON) of the abr_tree.
 */
char *abr_tree_to_string(abr_tree *t);

/*
 * Same as abr_tree_to_string(), but successful leaves have their text
 * printed, instead of the "[]" standing for "no children".
 * Useful when debugging a parser.
 */
char *abr_tree_to_string_with_leaves(const char *input, abr_tree *t);

/*
 * Returns a copy of the string behind the abr_tree.
 */
char *abr_tree_string(const char *input, abr_tree *t);

//
// abr_parser builders

abr_parser *abr_string(const char *s);
abr_parser *abr_regex(const char *s);
abr_parser *abr_regex_r(regex_t *r);

abr_parser *abr_rep(abr_parser *p, int min, int max);
abr_parser *abr_alt(abr_parser *p, ...);
abr_parser *abr_seq(abr_parser *p, ...);
abr_parser *abr_not(abr_parser *p);

abr_parser *abr_n_alt(const char *name, abr_parser *p, ...);
abr_parser *abr_n_regex(const char *name, const char *s);
abr_parser *abr_n_regex_r(const char *name, regex_t *r);
abr_parser *abr_n_rep(const char *name, abr_parser *p, int min, int max);
abr_parser *abr_n_seq(const char *name, abr_parser *p, ...);
abr_parser *abr_n_string(const char *name, const char *s);

abr_parser *abr_name(const char *name, abr_parser *p);

abr_parser *abr_presence(abr_parser *p);
abr_parser *abr_absence(abr_parser *p);

abr_parser *abr_n(const char *name);

//
// entry point

abr_tree *abr_parse_all(const char *input, size_t offset, abr_parser *p);
abr_tree *abr_parse(const char *input, size_t offset, abr_parser *p);

//
// helper functions

char *abr_error_message(abr_tree *t);

/*
 * The model for a function that, given a tree, returns an integer.
 *
 * -1: no, don't go on with my children
 *  0: no, but please go on with my children if I have any
 *  1: success (collect me, but not any of my children)
 */
typedef int abr_tree_func(abr_tree *);

/*
 * given a tree (starting point) and an abr_tree_func, collects all the
 * [sub-trees] that return 1 when the function is called on them.
 */
abr_tree **abr_tree_collect(abr_tree *t, abr_tree_func *f);

#endif // AABRO_H

