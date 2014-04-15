
//
// specifying aabro
//
// Mon Apr 14 06:28:07 JST 2014
//

#include "aabro.h"


context "parser"
{
  before each
  {
    abr_tree *t = NULL;
    abr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
    if (p != NULL) abr_parser_free(p);
  }

  it "parses everything"
  {
    p = abr_rep(abr_string("x"), 1, 4);
    t = abr_parse("x", 0, p);
    char *s = abr_tree_to_string(t);

    ensure(s ===f ""
      "[ null, 1, 0, 1, \"rep\", [\n"
      "  [ null, 1, 0, 1, \"string\", [] ],\n"
      "  [ null, 0, 1, -1, \"string\", [] ]\n"
      "] ]");
  }

  it "fails when there is too much to parse"
  {
    p = abr_rep(abr_string("x"), 1, 2);
    t = abr_parse("xxy", 0, p);
    char *s = abr_tree_to_string(t);

    ensure(s ===f ""
      //"[ null, 1, 0, 2, [\n"
      "[ null, 0, 0, 2, \"rep\", [\n"
      "  [ null, 1, 0, 1, \"string\", [] ],\n"
      "  [ null, 1, 1, 1, \"string\", [] ]\n"
      "] ]");
  }
}

