
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

int MAX_REPS = 2048; // TODO: decide
int MAX_P_CHILDREN = 64;


abr_tree *abr_tree_malloc(
  int success, int offset, int length, abr_parser *p, abr_tree **children
)
{
  abr_tree *t = malloc(sizeof(abr_tree));

  t->name = (p->name == NULL) ? NULL : strdup(p->name);
  t->success = success;
  t->offset = offset;
  t->length = length;
  t->parser = p;
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

char *abr_p_names[] = { // const ?
  "string", "regex",
  "rep", "alt", "seq",
  "not", "name", "presence", "absence", "n"
};

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
  if (t->name == NULL) flu_sbprintf(b, "null");
  else flu_sbprintf(b, "\"%s\"", t->name);
  //
  flu_sbprintf(
    b,
    ", %d, %d, %d, \"%s\", [",
    t->success, t->offset, t->length, abr_p_names[t->parser->type]);

  if (t->children == NULL)
  {
    flu_sbprintf(b, "] ]");
    return;
  }

  for (size_t i = 0; ; i++)
  {
    if (t->children[i] == NULL) break;
    if (i > 0) flu_sbprintf(b, ",");
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
  if (p->name != NULL)
  {
    free(p->name);
  }

  // free the regex if it was created with abr_regex_s(char *s)
  if (p->regex != NULL && p->string != NULL)
  {
     regfree(p->regex);
     free(p->regex);
  }

  if (p->string != NULL)
  {
    free(p->string);
  }

  if (p->children != NULL)
  {
    // do not free children themselves for abr_n()...

    if (p->type != 9) for(size_t i = 0; ; i++)
    {
      if (p->children[i] == NULL) break;
      abr_parser_free(p->children[i]);
    }
    free(p->children);
  }
  free(p);
}

abr_parser *abr_parser_malloc(unsigned short type, char *name)
{
  abr_parser *p = malloc(sizeof(abr_parser));

  p->name = (name == NULL) ? NULL : strdup(name);
  p->type = type;
  p->string = NULL;
  p->string_length = -1;
  p->regex = NULL;
  p->min = -1; p->max = -1;
  p->children = NULL;

  return p;
}

//
// the builder methods

void abr_do_name(abr_parser *named, abr_parser *target)
{
  if (named->name == NULL) return;
  if (target->children == NULL) return;

  for (size_t i = 0; ; i++)
  {
    abr_parser *child = target->children[i];

    if (child == NULL) break;

    if (child->type == 9 && strcmp(child->name, named->name) == 0)
    {
      //target->children[i] = named;
      //abr_parser_free(child);
      child->children[0] = named;
    }
    else
    {
      abr_do_name(named, child);
    }
  }
}

void abr_list_children(abr_parser *p, abr_parser *child0, va_list ap)
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
 * placeholder (abr_n)
 */

abr_parser *abr_string(char *s)
{
  return abr_n_string(NULL, s);
}

abr_parser *abr_n_string(char *name, char *s)
{
  abr_parser *p = abr_parser_malloc(0, name);
  p->string = strdup(s);
  p->string_length = strlen(s);
  return p;
}

abr_parser *abr_regex(char *s)
{
  return abr_n_regex(NULL, s);
}

abr_parser *abr_n_regex(char *name, char *s)
{
  abr_parser *p = abr_parser_malloc(1, name);
  p->string = strdup(s); // keep a copy of the original
  p->regex = malloc(sizeof(regex_t));
  regcomp(p->regex, p->string, REG_EXTENDED);
  return p;
}

abr_parser *abr_regex_r(regex_t *r)
{
  return abr_n_regex_r(NULL, r);
}

abr_parser *abr_n_regex_r(char *name, regex_t *r)
{
  abr_parser *p = abr_parser_malloc(1, name);
  p->regex = r;
  return p;
}

abr_parser *abr_rep(abr_parser *p, int min, int max)
{
  return abr_n_rep(NULL, p, min, max);
}

abr_parser *abr_n_rep(char *name, abr_parser *p, int min, int max)
{
  abr_parser *r = abr_parser_malloc(2, name);
  r->min = min;
  r->max = max;
  r->children = calloc(2, sizeof(abr_parser *));
  r->children[0] = p;
  r->children[1] = NULL;
  abr_do_name(r, r);
  return r;
}

abr_parser *abr_alt(abr_parser *p, ...)
{
  abr_parser *r = abr_parser_malloc(3, NULL);

  va_list l; va_start(l, p); abr_list_children(r, p, l); va_end(l);

  return r;
}

abr_parser *abr_n_alt(char *name, abr_parser *p, ...)
{
  abr_parser *r = abr_parser_malloc(3, name);

  va_list l; va_start(l, p); abr_list_children(r, p, l); va_end(l);
  abr_do_name(r, r);

  return r;
}

abr_parser *abr_seq(abr_parser *p, ...)
{
  abr_parser *r = abr_parser_malloc(4, NULL);

  va_list l; va_start(l, p); abr_list_children(r, p, l); va_end(l);

  return r;
}

abr_parser *abr_n_seq(char *name, abr_parser *p, ...)
{
  abr_parser *r = abr_parser_malloc(4, name);

  va_list l; va_start(l, p); abr_list_children(r, p, l); va_end(l);
  abr_do_name(r, r);

  return r;
}

abr_parser *abr_name(char *name, abr_parser *p)
{
  abr_parser *r = abr_parser_malloc(6, name);
  r->children = calloc(2, sizeof(abr_parser *));
  r->children[0] = p;
  //r->children[1] = NULL;
  abr_do_name(r, r);
  return r;
}

abr_parser *abr_n(char *name)
{
   abr_parser *r = abr_parser_malloc(9, name);
   r->children = calloc(2, sizeof(abr_parser *));
   return r;
}

//
// the to_s methods

typedef void abr_p_to_s_func(flu_sbuffer *, int, abr_parser *);

void abr_p_to_s(flu_sbuffer *b, int indent, abr_parser *p);

void abr_p_string_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  if (p->name == NULL) flu_sbprintf(b, "abr_string(\"%s\")", p->string);
  else flu_sbprintf(b, "abr_n_string(\"%s\", \"%s\")", p->name, p->string);
}

void abr_p_regex_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  if (p->string == NULL)
  {
    if (p->name == NULL) flu_sbprintf(b, "abr_regex_r(%p)", p->regex);
    else flu_sbprintf(b, "abr_n_regex_r(\"%s\", %p)", p->name, p->regex);
  }
  else
  {
    if (p->name == NULL) flu_sbprintf(b, "abr_regex(\"%s\")", p->string);
    else flu_sbprintf(b, "abr_n_regex(\"%s\", \"%s\")", p->name, p->string);
  }
}

void abr_p_rep_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  if (p->name == NULL)
  {
    flu_sbprintf(b, "abr_rep(\n");
  }
  else
  {
    flu_sbprintf(b, "abr_n_rep(\n");
    for (int i = 0; i < indent + 1; i++) flu_sbprintf(b, "  ");
    flu_sbprintf(b, "\"%s\",\n", p->name);
  }
  abr_p_to_s(b, indent + 1, p->children[0]);
  flu_sbprintf(b, ", %i, %i)", p->min, p->max);
}

void abr_p_wchildren_to_s(char *n, flu_sbuffer *b, int indent, abr_parser *p)
{
  if (p->name == NULL)
  {
    flu_sbprintf(b, "abr_%s(\n", n);
  }
  else
  {
    flu_sbprintf(b, "abr_n_%s(\n", n);
    for (int i = 0; i < indent + 1; i++) flu_sbprintf(b, "  ");
    flu_sbprintf(b, "\"%s\",\n", p->name);
  }
  for (size_t i = 0; ; i++)
  {
    abr_p_to_s(b, indent + 1, p->children[i]);
    if (p->children[i] == NULL) break;
    flu_sbprintf(b, ",\n");
  }
  flu_sbprintf(b, ")");
}

void abr_p_alt_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  abr_p_wchildren_to_s("alt", b, indent, p);
}

void abr_p_seq_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  abr_p_wchildren_to_s("seq", b, indent, p);
}

void abr_p_not_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
}

void abr_p_name_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  flu_sbprintf(b, "abr_name(\n");
  for (int i = 0; i < indent + 1; i++) flu_sbprintf(b, "  ");
  flu_sbprintf(b, "\"%s\",\n", p->name);
  abr_p_to_s(b, indent + 1, p->children[0]);
  flu_sbprintf(b, ")");
}

void abr_p_presence_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
}

void abr_p_absence_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
}

void abr_p_n_to_s(flu_sbuffer *b, int indent, abr_parser *p)
{
  flu_sbprintf(b, "abr_n(\"%s\")", p->name);
  if (p->children[0] == NULL) flu_sbprintf(b, " /* not linked */", p->name);
  //else flu_sbprintf(b, " /* linked */", p->name);
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
  abr_p_absence_to_s,
  abr_p_n_to_s
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
  int le = p->string_length;

  int su = 1;

  if (strncmp(input + offset, s, le) != 0) { su = 0; le = -1; }

  //free(s);
    // no, it's probably a string literal...
    // let the caller free it if necessary

  return abr_tree_malloc(su, offset, le, p, NULL);
}

abr_tree *abr_p_regex(char *input, int offset, abr_parser *p)
{
  regmatch_t ms[1];

  if (regexec(p->regex, input + offset, 1, ms, 0))
  {
    // failure
    return abr_tree_malloc(0, offset, -1, p, NULL);
  }

  // success
  return abr_tree_malloc(1, offset, ms[0].rm_eo - ms[0].rm_so, p, NULL);
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

  return abr_tree_malloc(success, offset, length, p, children);
}

abr_tree *abr_p_alt(char *input, int offset, abr_parser *p)
{
  size_t c = 0; while(1) if (p->children[c++] == NULL) break;
  abr_tree **ts = calloc(c, sizeof(abr_tree *));

  int success = 0;
  int length = -1;

  for (size_t i = 0; i < c - 1; i++)
  {
    ts[i] = abr_parse(input, offset, p->children[i]);
    if ( ! ts[i]->success) continue;
    success = 1;
    length = ts[i]->length;
    break;
  }

  return abr_tree_malloc(success, offset, length, p, ts);
}

abr_tree *abr_p_seq(char *input, int offset, abr_parser *p)
{
  size_t c = 0; while(1) if (p->children[c++] == NULL) break;
  abr_tree **ts = calloc(c, sizeof(abr_tree *));

  int success = 1;
  int length = 0;
  int off = offset;

  for (size_t i = 0; i < c - 1; i++)
  {
    ts[i] = abr_parse(input, off, p->children[i]);
    if ( ! ts[i]->success) { success = 0; length = -1; break; }
    off += ts[i]->length;
    length += ts[i]->length;
  }

  return abr_tree_malloc(success, offset, length, p, ts);
}

abr_tree *abr_p_not(char *input, int offset, abr_parser *p)
{
  return NULL;
}

abr_tree *abr_p_name(char *input, int offset, abr_parser *p)
{
  abr_tree *t = abr_parse(input, offset, p->children[0]);
  t->name = strdup(p->name);

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

abr_tree *abr_p_n(char *input, int offset, abr_parser *p)
{
  // When a placeholder is called for parsing, it means the parser tree
  // is incomplete. Do fail.

  return abr_tree_malloc(0, offset, -1, p, NULL);
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
  abr_p_absence,
  abr_p_n
};

abr_tree *abr_parse(char *input, int offset, abr_parser *p)
{
  return abr_p_funcs[p->type](input, offset, p);
}

abr_tree *abr_parse_all(char *input, int offset, abr_parser *p)
{
  abr_tree *t = abr_parse(input, offset, p);
  if (t->success && t->length != strlen(input)) t->success = 0;

  return t;
}

