
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

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "flutil.h"
#include "aabro.h"


abr_tree *abr_tree_malloc(int success, int offset, int length)
{
  abr_tree *t = malloc(sizeof(abr_tree));

  t->name = NULL;
  t->success = success;
  t->offset = offset;
  t->length = length;
  t->children = NULL;

  return t;
}

void abr_tree_free(abr_tree *t)
{
  if (t == NULL) return;
  if (t->name != NULL) free(t->name);
  free(t);
}

char *abr_tree_to_string(abr_tree *t)
{
  return flu_sprintf(
    "[ %s, %d, %d, %d ]",
    t->name,
    t->success,
    t->offset,
    t->length);
}

//
// the abr_parser methods

void abr_parser_free(abr_parser *p)
{
  if (p == NULL) return;

  if (p->string != NULL) free(p->string);
  //if (p->regex != NULL) regfree(p->regex);

  if (p->children != NULL)
  {
    for(size_t i = 0; ; i++)
    {
      if (p->children[i] == NULL) break;
      abr_parser_free(p->children[i]);
    }
  }
  free(p);
}

abr_parser *abr_parser_malloc(
  unsigned short type, char *string, int min, int max
)
{
  abr_parser *p = malloc(sizeof(abr_parser));

  p->type = type;
  p->string = NULL;
  if (string != NULL) p->string = strdup(string);
  //p->regex = NULL;
  p->min = min; p->max = max;
  p->children = NULL;

  return p;
}

char *abr_parser_to_string(abr_parser *p)
{
  return strdup("");
}

//
// the builder methods

/*
 * type 0 string
 * type 1 char
 * type 2 regex
 * type 3 rep
 * type 4 alternative
 * type 5 not, negation
 * type 6 name
 * type 7 presence
 * type 8 absence
 */

abr_parser *abr_string(char *s)
{
  return abr_parser_malloc(0, s, -1, -1);
}

//
// the parse methods

typedef abr_tree *abr_p_func(char *, int, abr_parser *);

abr_tree *abr_p_string(char *input, int offset, abr_parser *p)
{
  char *s = p->string;

  int su = 1;
  int le = strlen(s);

  if (strncmp(input + offset, s, le) != 0) { su = 0; le = -1; }

  //free(s);
    // no, it's probably a string literal...
    // let the caller free it if necessary

  return abr_tree_malloc(su, offset, le);
}

abr_tree *abr_p_char(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_regex(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_rep(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_alt(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_not(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_name(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_presence(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_absence(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_p_func *abr_p_funcs[] = { // const ?
  abr_p_string,
  abr_p_char,
  abr_p_regex,
  abr_p_rep,
  abr_p_alt,
  abr_p_not,
  abr_p_name,
  abr_p_presence,
  abr_p_absence
};

abr_tree *abr_parse(char *input, int offset, abr_parser *p)
{
  return NULL;
}

