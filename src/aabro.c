
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

static fabr_tree *str(fabr_input *i, char *rx, size_t rxn)
{
  fabr_tree *r = fabr_tree_malloc(NULL, "str", i);

  if (strncmp(i->string + i->offset, rx, rxn) != 0)
    r->result = 0;
  else
    r->length = rxn;

  return r;
}

fabr_tree *fabr_str(
  char *name, fabr_input *i, char *s)
{
  fabr_tree *r = str(i, s, strlen(s));

  r->name = name ? strdup(name) : NULL;

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

static char rx_at(char *rx, size_t rxn, size_t index)
{
  return rxn > index ? rx[index] : 0;
}

static char *rx_chr(char *rx, size_t rxn, int c)
{
  char *r = strchr(rx, c);

  return r - rx >= rxn ? NULL : r;
}

//typedef fabr_tree *fabr_rex_parser(fabr_input *i, char *rx, size_t rxn);

static void rng_next(char *rx, size_t rxn, char *next)
{
  size_t b_index = 1;

  char a = rx_at(rx, rxn, 0);
  if (a == '\\') { a = rx_at(rx, rxn, 1); b_index = 2; }
  if (a == '\0') { next[0] = 0; next[1] = 0; next[2] = 0; return; }

  char b = rx_at(rx, rxn, b_index);
  char c = (b != '\0') ? rx_at(rx, rxn, b_index + 1) : 'X'; // don't go over
  if (b != '-' || c == '\0') { next[0] = 1; next[1] = a; next[2] = a; return; }

  b = rx_at(rx, rxn, ++b_index);
  if (b == '\\') b = rx_at(rx, rxn, ++b_index);
  next[0] = 2; next[1] = a; next[2] = b;
}

static fabr_tree *rng(fabr_input *i, char *rx, size_t rxn)
{
  fabr_tree *r = fabr_tree_malloc(NULL, "rng", i);

  char c = (i->string + i->offset)[0];
  char irc = rxn > 0 ? rx[0] : 0;

  if (irc == '$') { r->result = (c == '\0'); return r; }
  if (c == '\0') { r->result = 0; return r; }

  if (irc == '.')
  {
    if (c == '\n') r->result = 0; else r->length = 1;
    return r;
  }

  r->result = 0;

  short not = (irc == '^'); if (not) { rx++; rxn--; }

  char next[] = { 0, 0, 0 };
  while (1)
  {
    rng_next(rx, rxn, next);
    if (next[0] == 0) break;
    if (c >= next[1] && c <= next[2]) { r->result = 1; break; }
    rx += next[0]; rxn -= next[0];
  }

  if (not) r->result = ( ! r->result);
  r->length = r->result ? 1 : 0;

  return r;
}

fabr_tree *fabr_rng(
  char *name, fabr_input *i, char *range)
{
  fabr_tree *r = rng(i, range, strlen(range));

  r->name = name ? strdup(name) : NULL;

  return r;
}

static fabr_tree *ferr(fabr_input *i, char *parter, char *format, ...)
{
  fabr_tree *r = fabr_tree_malloc(NULL, parter, i);

  va_list ap; va_start(ap, format);
  r->note = flu_svprintf(format, ap);
  va_end(ap);

  return r;
}

static size_t quantify(char *rx, size_t rxn, size_t *reps)
{
  if (reps == NULL) reps = (size_t []){ 0, 0 };

  char c = rx_at(rx, rxn, 0);

  if (c == '?') { reps[0] = 0; reps[1] = 1; return 1; }
  if (c == '*') { reps[0] = 0; reps[1] = 0; return 1; }
  if (c == '+') { reps[0] = 1; reps[1] = 0; return 1; }

  char *end = rx_chr(rx, rxn, '}'); if (end == NULL) return 0; // error

  reps[0] = strtol(rx + 1, NULL, 10);

  char *comma = rx_chr(rx, rxn, ',');

  reps[1] = comma == NULL ? reps[0] : strtol(comma + 1, NULL, 10);
  return end - rx;
}

static fabr_tree *rex_elt(fabr_input *i, char *rx, size_t rxn)
{
  printf("    rex_elt() >%s< %zu\n", rx, rxn);
  //printf("    rex_elt() >%.*s< %zu\n", rxn, rx, rxn);

  // if it begins with a [ it's a range
  // if it begins with a ( it's a group
  // else it's a string

  // TODO deal with quantifier (suffix)

  return str(i, rx, rxn);
}

//static size_t find_range_end(char *rx, size_t rxn)
//{
//  for (size_t i = 1; i < rxn; i++)
//  {
//    char c = rx_at(rx, rxn, i);
//
//    if (c == '\0') break;
//    if (c == '\\') { i++; continue; }
//    if (c != ']') continue;
//
//    size_t ql = quantify(rx + i + 1, rxn - i - 1, NULL);
//
//    return i + 1 + ql;
//  }
//
//  return 0;
//}

static fabr_tree *rex_seq(fabr_input *i, char *rx, size_t rxn)
{
  printf("  rex_seq() >%s< %zu\n", rx, rxn);

  // a sequence is a sequence of
  //
  // a string of characters without a quantifier  abcd* -> abc
  // a character with a quantifier or not  d*
  // a range with a quantifier or not  [a-b]+
  // a group with a quantifier or not  (truc)?

  fabr_tree *r = fabr_tree_malloc(NULL, "rex_seq", i);

  fabr_tree *prev = NULL;
  fabr_tree **next = &r->child;

  char c = 'a';
  char *crx = rx;
  size_t crxn = rxn;

  // OR
  //
  // two passes
  //
  // 1) ab[cd]+ef*(gh|ij)?k --> ab [cd] + e f * (gh|ij) ? k
  // 2) iterate the 1) list
  //
  // OR
  //
  // well, the one pass idea is not too bad...
  // but need a way to pass the computed quantifier info to rex_elt()

  // AND if I only cared about quantifiers?
  // NO, I have to care about transitions
  //   ab[cd]+ef*(gh|ij)?k --> ab [cd] + e f * (gh|ij) ? k
  // * start of range -> til end of range + quantifier
  // * start of group -> til end of group + quantifier
  // * [lonely] quantifier
  // * end of rx

  while (1)
  {
    size_t l = 0;
    fabr_tree *t = rex_elt(i, rx, rxn, &l);
    r->rexlen += t->rexlen;

    // TODO continue me

    break;
  }

//  do
//  {
//    for (size_t j = 0; ; j++)
//    {
//      c = j >= crxn ? 0 : crx[j];
//
//      if (c == '\0')
//      {
//        *next = rex_elt(i, crx, j); prev = *next;
//        break;
//      }
//
//      if (c == '\\') { j++; continue; }
//
//      // if start of range
//      // if start of group
//
//      // if end of range
//      // if end of group
//      // if quantifier
//
//      // abc+
//      size_t ql = quantify(crx + j, crxn - j, NULL);
//      if (ql > 0)
//      {
//        *next = rex_elt(i, crx, j - 1); // ab
//        prev = *next; next = &prev->sibling;
//
//        *next = rex_elt(i, crx + j - 1, 1 + ql); // c+
//        prev = *next; next = &prev->sibling;
//
//        crx = crx + j + ql; crxn = crxn - j - ql;
//
//        break;
//      }
//
//      if (c == '[' || c == '(')
//      {
//        // parse what came before if j > 0
//
//        if (j > 0)
//        {
//        }
//        else
//        {
//        }
//
//        break;
//      }
//
////      if (c == '[' || c == '(')
////      {
////        // TODO parse based on what came previously, without quantifier
////        // NO, nested group...
////      }
////
////      if (c == '?' || c == '*' || c == '+' || c == '{')
////      {
////        // TODO parse based on what came previously, with quantifier
////
////        size_t reps[] = { 1, 1 };
////        size_t l = quantify(crx + j, xxx, reps);
////
////        if (l == 0)
////        {
////          *next = ferr(i, "quantifier not closed >%s<", crx + j);
////          prev = *next;
////          break;
////        }
////      }
//    }
//
//    if (prev->result != 1) break;
//    r->length += prev->length;
//
//  } while (c != 0);
//
//  if (prev->result != 1) { r->result = prev->result; r->length = 0; }
//
//  return r;
}

static fabr_tree *rex_alt(fabr_input *i, char *rx, size_t rxn)
{
  fabr_tree *r = fabr_tree_malloc(NULL, "rex_alt", i);

  // TODO/WARNING: greedy, the wants to have the longest match...

  fabr_tree *prev = NULL;
  fabr_tree **next = &r->child;

  char c = 'a';
  char *crx = rx;
  size_t crxn = rxn;

  do
  {
    printf("rex_alt() >%s< %zu c%i\n", crx, crxn, c);

    for (size_t j = 0, range = 0, groups = 0; ; j++)
    {
      c = j >= crxn ? 0 : crx[j];

      if (c == '\0')
      {
        *next = rex_seq(i, crx, j);
        prev = *next;
        //next = &prev->sibling;
        break;
      }

      if (c == '\\') { i++; continue; }

      if (c == '[') { range = 1; continue; }
      if (range && c == ']') { range = 0; continue; }
      if (range) continue;

      if (c == '(') { groups++; continue; }
      if (groups > 0 && c == ')') { groups--; continue; }

      if (c == '|')
      {
        *next = rex_seq(i, crx, j);
        prev = *next; next = &prev->sibling;
        //crx = crx + j + 1; crxn = crxn - j - 1;
        crx += prev->rexlen; crxn -= prev->rexlen;
        break;
      }

      // else continue
    }

    if (prev->result == 1) break;

  } while (c != 0);

  if (prev->result != 1)
    r->result = prev->result;
  else
    r->length = prev->length;

  return r;
}

fabr_tree *fabr_rex(
  char *name, fabr_input *i, char *regex)
{
  fabr_tree *r = rex_alt(i, regex, strlen(regex));

  r->name = name ? strdup(name) : NULL;

  return r;
}

