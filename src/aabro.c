
//
// Copyright (c) 2013-2015, John Mettraux, jmettraux+flon@gmail.com
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

// https://github.com/flon-io/aabro

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include "aabro.h"


fabr_tree *fabr_tree_malloc(char *name, char *parter, fabr_input *i)
{
  fabr_tree *t = calloc(1, sizeof(fabr_tree));

  t->name = name ? strdup(name) : NULL;
  t->result = 1;
  t->parter = parter;
  t->offset = i->offset;
  t->length = 0;
  t->note = NULL;
  t->sibling = NULL;
  t->child = NULL;

  return t;
}

void fabr_tree_free(fabr_tree *t)
{
  if (t == NULL) return;

  free(t->name);
  free(t->note);

  for (fabr_tree *c = t->child; c != NULL; )
  {
    fabr_tree *s = c->sibling;
    fabr_tree_free(c);
    c = s;
  }

  free(t);
}

char *fabr_tree_string(const char *input, fabr_tree *t)
{
  return strndup(input + t->offset, t->length);
}

char *fabr_tree_str(char *input, fabr_tree *t)
{
  return input + t->offset;
}

static void fabr_t_to_s(
  fabr_tree *t, const char *input,
  flu_sbuffer *b, size_t indent, short children, short color)
{
  for (size_t i = 0; i < indent; i++) flu_sbprintf(b, "  ");

  if (t == NULL) { flu_sbprintf(b, "null"); return; }

  char *stringc = color ? "[1;33m" : ""; // yellow
  char *clearc = color ? "[0;0m" : "";
  char *namec = color ? "[1;34m" : ""; // light blue
  char *notec = color ? "[1;31m" : ""; // light red

  char *name = "null";
  char *note = "null";
  char *resultc = ""; if (color) resultc = t->result ? "[0;0m" : "[1;30m";
  if (t->name) name = flu_sprintf("\"%s%s%s\"", namec, t->name, resultc);
  if (t->note) note = flu_sprintf("\"%s%s%s\"", notec, t->note, resultc);
  //
  flu_sbprintf(
    b,
    "%s[ %s, %d, %d, %d, %s, \"%s\", ",
    resultc, name, t->result, t->offset, t->length, note, t->parter);
  //
  if (t->name) free(name);
  if (t->note) free(note);

  if (children != 1 && (input == NULL || t->result != 1 || t->child))
  {
    size_t cc = 0; for (fabr_tree *c = t->child; c; c = c->sibling) ++cc;
    flu_sbprintf(b, "%zu ]%s", cc, clearc);
    return;
  }

  if (t->child == NULL)
  {
    if (input == NULL || t->result != 1)
    {
      flu_sbprintf(b, "[] ]%s", clearc);
    }
    else
    {
      char *s = flu_n_escape(input + t->offset, t->length);
      flu_sbprintf(b, "\"%s%s%s\" ]%s", stringc, s, resultc, clearc);
      free(s);
    }
    return;
  }

  flu_sbprintf(b, "[");

  for (fabr_tree *c = t->child; c != NULL; c = c->sibling)
  {
    if (c != t->child) flu_sbputc(b, ',');
    flu_sbputc(b, '\n');
    fabr_t_to_s(c, input, b, indent + 1, children, color);
  }

  flu_sbputc(b, '\n');
  for (int i = 0; i < indent; i++) flu_sbprintf(b, "  ");
  flu_sbprintf(b, "%s] ]%s", resultc, clearc);
}

char *fabr_tree_to_string(fabr_tree *t, const char *input, short color)
{
  flu_sbuffer *b = flu_sbuffer_malloc();
  fabr_t_to_s(t, input, b, 0, 1, color);
  return flu_sbuffer_to_string(b);
}

char *fabr_tree_to_str(fabr_tree *t, const char *input, short color)
{
  flu_sbuffer *b = flu_sbuffer_malloc();
  fabr_t_to_s(t, input, b, 0, 0, color);
  return flu_sbuffer_to_string(b);
}


//
// helper functions

char *fabr_error_message(fabr_tree *t)
{
  if (t->result == -1 && t->note != NULL) return t->note;

  for (fabr_tree *c = t->child; c != NULL; c = c->sibling)
  {
    char *s = fabr_error_message(c);
    if (s != NULL) return s;
  }

  return NULL;
}

fabr_tree *fabr_tree_lookup(fabr_tree *t, const char *name)
{
  return fabr_subtree_lookup(&((fabr_tree){ .child = t }), name);
}

fabr_tree *fabr_subtree_lookup(fabr_tree *t, const char *name)
{
  for (fabr_tree *c = t->child; c != NULL; c = c->sibling)
  {
    if (name == NULL && c->name != NULL) return c;
    if (name && c->name && strcmp(c->name, name) == 0) return c;

    fabr_tree *r = fabr_subtree_lookup(c, name);
    if (r) return r;
  }

  return NULL;
}

char *fabr_lookup_string(const char *input, fabr_tree *t, const char *name)
{
  fabr_tree *tt = fabr_tree_lookup(t, name);

  return tt ? fabr_tree_string(input, tt) : NULL;
}

static void fabr_t_list(flu_list *l, fabr_tree *t, fabr_tree_func *f)
{
  short r = f(t);

  if (r < 0) { return; }
  if (r > 0) { flu_list_add(l, t); return; }

  for (fabr_tree *c = t->child; c != NULL; c = c->sibling)
  {
    fabr_t_list(l, c, f);
  }
}

flu_list *fabr_tree_list(fabr_tree *t, fabr_tree_func *f)
{
  flu_list *l = flu_list_malloc();

  fabr_t_list(l, t, f);

  return l;
}

static void fabr_t_list_named(flu_list *l, fabr_tree *t, const char *name)
{
  if (t->result != 1) { return; }
  if (t->name && strcmp(t->name, name) == 0) { flu_list_add(l, t); return; }

  for (fabr_tree *c = t->child; c != NULL; c = c->sibling)
  {
    fabr_t_list_named(l, c, name);
  }
}

flu_list *fabr_tree_list_named(fabr_tree *t, const char *name)
{
  flu_list *l = flu_list_malloc();

  fabr_t_list_named(l, t, name);

  return l;
}

fabr_tree **fabr_tree_collect(fabr_tree *t, fabr_tree_func *f)
{
  flu_list *l = fabr_tree_list(t, f);

  fabr_tree **ts = (fabr_tree **)flu_list_to_array(l, FLU_F_EXTRA_NULL);
  flu_list_free(l);

  return ts;
}

fabr_tree *fabr_t_child(fabr_tree *t, size_t index)
{
  for (fabr_tree *c = t->child; c != NULL; c = c->sibling)
  {
    if (index == 0) return c;
    --index;
  }

  return NULL;
}


//
// parters (partial parsers)

fabr_tree *fabr_str(
  char *name, fabr_input *i, char *str)
{
  size_t l = strlen(str);

  fabr_tree *r = fabr_tree_malloc(name, "str", i);

  if (strncmp(i->string + i->offset, str, l) != 0)
    r->result = 0;
  else
    r->length = l;

  return r;
}

fabr_tree *fabr_seq(
  char *name, fabr_input *i, fabr_parser *p, ...)
{
  fabr_tree *r = fabr_tree_malloc(name, "seq", i);

  fabr_tree **next = &r->child;

  va_list ap; va_start(ap, p);
  while (1)
  {
    fabr_tree *t = p(i);
    *next = t;

    if (t->result != 1) { r->result = 0; r->length = 0; break; }

    r->length += t->length;
    i->offset += t->length;

    p = va_arg(ap, fabr_parser *); if (p == NULL) break;
    next = &(t->sibling);
  }
  va_end(ap);

  return r;
}

fabr_tree *fabr_alt(
  char *name, fabr_input *i, fabr_parser *p, ...)
{
  fabr_tree *r = fabr_tree_malloc(name, "alt", i);
  r->result = 0;

  fabr_tree **next = &r->child;

  va_list ap; va_start(ap, p);
  while (1)
  {
    fabr_tree *t = p(i);
    *next = t;

    if (t->result == 1) { r->result = 1; r->length = t->length; break; }

    p = va_arg(ap, fabr_parser *); if (p == NULL) break;
    next = &(t->sibling);
  }
  va_end(ap);

  return r;
}

fabr_tree *fabr_rep(
  char *name, fabr_input *i, fabr_parser *p, size_t min, size_t max)
{
  fabr_tree *r = fabr_tree_malloc(name, "rep", i);

  fabr_tree **next = &r->child;
  size_t count = 0;

  while (1)
  {
    if (*(i->string + i->offset) == 0) break; // EOS

    fabr_tree *t = p(i);
    *next = t;

    if (t->result == 0) { r->result = 0; break; }

    i->offset += t->length;
    r->length += t->length;

    if (++count == max) break;

    next = &(t->sibling);
  }

  if (count < min) r->result = 0;
  if (r->result != 1) r->length = 0;

  return r;
}

static char irex_char_at(fabr_input *i, size_t index)
{
  return index >= i->rexn ? 0 : i->rex[index];
}
static void irex_increment(fabr_input *i, size_t inc)
{
  i->rex += inc; i->rexn -= inc;
}

static void rng_next(fabr_input *i, char *next)
{
  size_t b_index = 1;
  char a = irex_char_at(i, 0);
  if (a == '\\') { a = irex_char_at(i, 1); b_index = 2; }
  if (a == '\0') { next[0] = 0; next[1] = 0; next[2] = 0; return; }

  char b = irex_char_at(i, b_index);
  char c = (b != '\0') ? irex_char_at(i, b_index + 1) : 'X'; // don't go too far
  if (b != '-' || c == '\0') { next[0] = 1; next[1] = a; next[2] = a; return; }

  b = irex_char_at(i, ++b_index);
  if (b == '\\') b = irex_char_at(i, ++b_index);
  next[0] = 2; next[1] = a; next[2] = b;
}

static fabr_tree *rng(fabr_input *i)
{
  fabr_tree *r = fabr_tree_malloc(NULL, "rng", i);

  char c = (i->string + i->offset)[0];
  char irc = irex_char_at(i, 0);

  if (irc == '$') { r->result = (c == '\0'); return r; }
  if (c == '\0') { r->result = 0; return r; }

  if (irc == '.')
  {
    if (c == '\n') r->result = 0; else r->length = 1;
    return r;
  }

  r->result = 0;

  short not = (irc == '^'); if (not) irex_increment(i, 1);

  char next[] = { 0, 0, 0 };
  while (1)
  {
    rng_next(i, next);
    if (next[0] == 0) break;
    if (c >= next[1] && c <= next[2]) { r->result = 1; break; }
    irex_increment(i, next[0]);
  }

  if (not) r->result = ( ! r->result);
  r->length = r->result ? 1 : 0;

  return r;
}

static ssize_t find_range_end(fabr_input *i)
{
  for (size_t j = 0; ; ++j)
  {
    char c = irex_char_at(i, j);

    if (c == '\0') break;
    if (c == '\\') { ++j; continue; }
    if (c == ']') return j;
  }

  return -1;
}

static fabr_tree *error(fabr_input *i, char *parter, const char *format, ...)
{
  fabr_tree *r = fabr_tree_malloc(NULL, parter, i);

  r->result = -1;

  va_list ap; va_start(ap, format);
  r->note = flu_svprintf(format, ap);
  va_end(ap);

  return r;
}

static size_t determine_rminmax(fabr_input *i)
{
  printf("dr >%s< %zu\n", i->rex, i->rexn);
  if (i->rex == 0) return 0;
  if (*i->rex == '+') { i->rmax = 0; return 1; }
  if (*i->rex == '*') { i->rmin = 0; i->rmax = 0; return 1; }
  if (*i=>rex == '{') return 2; // FIXME
}

static fabr_tree *rex_range(fabr_input *i)
{
  // detect range end
  // determine repetition (defaults to 1, 1)
  // return rng() wrapped in rep()

  ssize_t end = find_range_end(i);

  if (end < 2) return error(i, "rng", "range not closed >%s<", i->rex);

  char *rex = i->rex + 1;
  size_t rexn = end - 1;

  //printf("_rng >%s< %zu\n", rex, rexn);

  irex_increment(i, end + 1);

  i->rmin = 1; i->rmax = 1; // exactly one
  size_t replen = determine_rminmax(i);

  //printf("_rng reps (%zu, %zu)\n", reps[0], reps[1]);

  i->rex = rex; i->rexn = rexn;

  if (reps[0] != 1 || reps[1] != 1)
  {
    i->rmin = reps[0]; i->
  }

  //else // exactly one, don't wrap

  return rng(i);
}

static fabr_tree *rex_group(fabr_input *i)
{
  // TODO
  return NULL;

  // eventually, rex() and rex_group() are the same thing
}

static fabr_tree *rex(fabr_input *i)
{
  fabr_tree *r = fabr_tree_malloc(NULL, "rex", i);

  fabr_tree **next = &(r->child);

  while (1)
  {
    char irc = irex_char_at(i, 0); if (irc == 0) break;

    fabr_tree *t = NULL;

    if (irc == '[') t = rex_range(i);
    else if (irc == '(') t = rex_group(i);
    //else ...

    *next = t;
    next = &(t->sibling);

    if (t == NULL) break;
    if (t->result != 1) { r->result = t->result; break; }

    r->length += t->length;

    break; // FIXME
  }

  return r;
}

fabr_tree *fabr_rng(
  char *name, fabr_input *i, char *range)
{
  i->rex = range;
  i->rexn = strlen(range);

  fabr_tree *r = rng(i);

  r->name = name ? strdup(name) : NULL;

  return r;
}

fabr_tree *fabr_rex(
  char *name, fabr_input *i, char *regex)
{
  i->rex = regex;
  i->rexn = strlen(regex);

  fabr_tree *r = rex(i);

  r->name = name ? strdup(name) : NULL;

  return r;
}

