
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
#include <stdarg.h>

#include "flutil.h"
#include "aabro.h"

int MAX_REPS = 2048; // TODO: decide later
int MAX_P_CHILDREN = 64;


abr_tree *abr_tree_malloc(
  int success, int offset, int length, abr_tree **children
)
{
  abr_tree *t = malloc(sizeof(abr_tree));

  t->name = NULL;
  t->success = success;
  t->offset = offset;
  t->length = length;
  t->children = children;

  return t;
}

void abr_tree_free(abr_tree *t)
{
  if (t->name != NULL)
  {
    free(t->name);
  }

  if (t->children != NULL)
  {
    for (size_t i = 0; ; i++)
    {
      if (t->children[i] == NULL) break;
      abr_tree_free(t->children[i]);
    }
    free(t->children);
  }

  free(t);
}

void abr_t_to_s(abr_tree *t, flu_sbuffer *b, int indent)
{
  for (int i = 0; i < indent; i++) flu_sbprintf(b, "  ");

  if (t == NULL)
  {
    flu_sbprintf(b, "{null}");
    return;
  }

  flu_sbprintf(b, "[ ");
  //
  if (t->name == NULL) flu_sbprintf(b, "(null)");
  else flu_sbprintf(b, "\"%s\"", t->name);
  //
  flu_sbprintf(b, ", %d, %d, %d, [", t->success, t->offset, t->length);

  if (t->children == NULL)
  {
    flu_sbprintf(b, "] ]");
    return;
  }

  for (size_t i = 0; ;i++)
  {
    if (t->children[i] == NULL) break;
    flu_sbprintf(b, "\n");
    abr_t_to_s(t->children[i], b, indent + 1);
  }

  flu_sbprintf(b, "\n] ]");
}

char *abr_tree_to_string(abr_tree *t)
{
  flu_sbuffer *b = flu_sbuffer_malloc();
  abr_t_to_s(t, b, 0);
  return flu_sbuffer_to_string(b);
}

//
// the abr_parser methods

void abr_parser_free(abr_parser *p)
{
  if (p->string != NULL) free(p->string);
  //if (p->regex != NULL) regfree(p->regex);

  if (p->children != NULL)
  {
    for(size_t i = 0; ; i++)
    {
      if (p->children[i] == NULL) break;
      abr_parser_free(p->children[i]);
    }
    free(p->children);
  }
  free(p);
}

abr_parser *abr_parser_malloc(unsigned short type)
{
  abr_parser *p = malloc(sizeof(abr_parser));

  p->type = type;
  p->string = NULL;
  //p->regex = NULL;
  p->min = -1; p->max = -1;
  p->children = NULL;

  return p;
}

//
// the builder methods

/*
 * string
 * regex
 * repetition
 * alternative
 * sequence
 * not, negation
 * name
 * presence
 * absence
 */

abr_parser *abr_string(char *s)
{
  abr_parser *p = abr_parser_malloc(0);
  p->string = strdup(s);
  return p;
}

abr_parser *abr_rep(abr_parser *p, int min, int max)
{
  abr_parser *r = abr_parser_malloc(2);
  r->min = min;
  r->max = max;
  r->children = calloc(2, sizeof(abr_parser *));
  r->children[0] = p;
  r->children[1] = NULL;
  return r;
}

void abr_set_children(abr_parser *p, abr_parser *child0, va_list ap)
{
  abr_parser **cs = calloc(MAX_P_CHILDREN, sizeof(abr_parser *));
  cs[0] = child0;
  size_t count = 1;

  while (1)
  {
    abr_parser *child = va_arg(ap, abr_parser *);
    if (child == NULL || count >= MAX_P_CHILDREN) break;
    cs[count++] = child;
  }

  abr_parser **children = calloc(count + 1, sizeof(abr_parser *));
  for (size_t i = 0; i < count; i++) children[i] = cs[i];

  free(cs);

  p->children = children;
}

abr_parser *abr_alt(abr_parser *p, ...)
{
  abr_parser *r = abr_parser_malloc(3);

  va_list ap;
  va_start(ap, p);
  abr_set_children(r, p, ap);
  va_end(ap);

  return r;
}

abr_parser *abr_name(char *name, abr_parser *p)
{
  abr_parser *r = abr_parser_malloc(6);
  r->string = strdup(name);
  r->children = calloc(2, sizeof(abr_parser *));
  r->children[0] = p;
  r->children[1] = NULL;
  return r;
}

//
// the to_s methods

typedef void abr_p_to_s_func(flu_sbuffer *, int, abr_parser *);

void abr_p_to_s(flu_sbuffer *b, int indent, abr_parser *p);

void abr_p_string_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  flu_sbprintf(b, "abr_string(\"%s\")", p->string);
}

void abr_p_regex_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
}

void abr_p_rep_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  flu_sbprintf(b, "abr_rep(\n");
  abr_p_to_s(b, indent + 1, p->children[0]);
  flu_sbprintf(b, ", %i, %i)", p->min, p->max);
}

void abr_p_alt_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  flu_sbprintf(b, "abr_alt(\n");
  for (size_t i = 0; ; i++)
  {
    abr_p_to_s(b, indent + 1, p->children[i]);
    if (p->children[i] == NULL) break;
    flu_sbprintf(b, ",\n");
  }
  flu_sbprintf(b, ")");
}

void abr_p_seq_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
}

void abr_p_not_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
}

void abr_p_name_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  flu_sbprintf(b, "abr_name(\"%s\",\n", p->string);
  abr_p_to_s(b, indent + 1, p->children[0]);
  flu_sbprintf(b, ")");
}

void abr_p_presence_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
}

void abr_p_absence_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
}

abr_p_to_s_func *abr_p_to_s_funcs[] = { // const ?
  abr_p_string_to_s,
  abr_p_regex_to_s,
  abr_p_rep_to_s,
  abr_p_alt_to_s,
  abr_p_seq_to_s,
  abr_p_not_to_s,
  abr_p_name_to_s,
  abr_p_presence_to_s,
  abr_p_absence_to_s
};

void abr_p_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  for (int i = 0; i < indent; i++) flu_sbprintf(b, "  ");
  if (p == NULL) flu_sbprintf(b, "NULL");
  else abr_p_to_s_funcs[p->type](b, 0, p);
}

char *abr_parser_to_string(abr_parser *p)
{
  flu_sbuffer *b = flu_sbuffer_malloc();
  abr_p_to_s(b, 0, p);
  return flu_sbuffer_to_string(b);
}

//
// the parse methods

typedef abr_tree *abr_p_func(char *, int, abr_parser *);

abr_tree *abr_p_string(char *input, int offset, abr_parser *p)
{
  char *s = p->string;

  int su = 1;
  int le = strlen(s); // would it make sense to cache that?

  if (strncmp(input + offset, s, le) != 0) { su = 0; le = -1; }

  //free(s);
    // no, it's probably a string literal...
    // let the caller free it if necessary

  return abr_tree_malloc(su, offset, le, NULL);
}

abr_tree *abr_p_regex(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_rep(char *input, int offset, abr_parser *p)
{
  int max = p->max;
  if (max < 0) max = MAX_REPS;
  int off = offset;
  size_t count = 0;
  int length = 0;
  abr_tree **reps = calloc(max + 1, sizeof(abr_tree *));

  for (size_t i = 0; i < p->max; i++)
  {
    count++;
    reps[i] = abr_parse(input, off, p->children[0]);
    if ( ! reps[i]->success) break;
    off += reps[i]->length;
    length += reps[i]->length;
  }
  int success = 1;
  if (count - 1 < p->min) success = 0;

  abr_tree **children = calloc(count + 1, sizeof(abr_tree *));
  for (size_t i = 0; i < count; i++) children[i] = reps[i];
  free(reps);

  return abr_tree_malloc(success, offset, length, children);
}

abr_tree *abr_p_alt(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_seq(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_not(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_name(char *input, int offset, abr_parser *p)
{
  abr_tree *t = abr_parse(input, offset, p->children[0]);
  t->name = strdup(p->string);

  return t;
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
  abr_p_regex,
  abr_p_rep,
  abr_p_alt,
  abr_p_seq,
  abr_p_not,
  abr_p_name,
  abr_p_presence,
  abr_p_absence
};

abr_tree *abr_parse(char *input, int offset, abr_parser *p)
{
  return abr_p_funcs[p->type](input, offset, p);
}

