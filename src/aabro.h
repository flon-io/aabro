
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

//
// abr_node

typedef struct abr_tree {
  char *name;
  int success;
  int offset;
  int length;
  struct abr_tree *children;
} abr_tree;

abr_tree *abr_tree_malloc(int success, int offset, int length);
void abr_tree_free(abr_node *n);

char *abr_tree_to_string(abr_tree *n);

//
// abr_parser_node

typedef struct abr_parser {
  char *name;
  char **parameters;
  struct abr_parser *children;
} abr_parser;

abr_tree *abr_p_string(char *input, int offset, abr_parser *p);

//
// abr_parser builders

abr_parser *abr_string(char *s);
abr_parser *abr_char(char c);
//abr_parser *abr_regex(regex_t *r);

abr_parser *abr_rep(abr_parser *n, int min, int max);
abr_parser *abr_alt(abr_parser *n, ...);
abr_parser *abr_not(abr_parser *n);

abr_parser *abr_name(char *name, abr_parser *n);

abr_parser *abr_presence(abr_parser *n);
abr_parser *abr_absence(abr_parser *n);

//
// entry

abr_tree *abr_parse(char *input, int offset, abr_parser *p);

#endif // AABRO_H

