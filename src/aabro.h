
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

typedef struct abr_node {
  char *name;
  int success;
  int offset;
  int length;
  struct abr_node *children;
} abr_node;

abr_node *abr_node_malloc(int success, int offset, int length);
void abr_node_free(abr_node *n);

char *abr_node_to_string(abr_node *n);

//
// abr_parser_node

typedef struct abr_parser_node {
  char *name;
  char **parameters;
  struct abr_parser_node *children;
} abr_parser_node;

abr_node *abr_p_string(char *input, int offset, abr_parser_node *p);

//
// abr_parser_node builders

abr_parser_node *abr_string(char *s);
abr_parser_node *abr_char(char c);
//abr_parser_node *abr_regex(regex_t *r);

abr_parser_node *abr_rep(abr_parser_node *n, int min, int max);
abr_parser_node *abr_alt(abr_parser_node *n, ...);
abr_parser_node *abr_not(abr_parser_node *n);

abr_parser_node *abr_name(char *name, abr_parser_node *n);

abr_parser_node *abr_presence(abr_parser_node *n);
abr_parser_node *abr_absence(abr_parser_node *n);

//
// entry

abr_node *abr_parse(char *input, int offset, abr_parser_node *parser);

#endif // AABRO_H

