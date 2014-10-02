
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
    fabr_tree *t = NULL;
    fabr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) fabr_tree_free(t);
    if (p != NULL) fabr_parser_free(p);
  }

  describe "fabr_parse(input, offset, parser)"
  {
    it "parses as much as it can"
    {
      p = fabr_rep(fabr_string("x"), 1, 4);
      t = fabr_parse("x", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"rep-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "is successful even if it didn't parse all the input"
    {
      p = fabr_rep(fabr_string("x"), 1, 2);
      t = fabr_parse("xxy", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 2, null, \"rep-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 1, 1, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "returns an error when recursion goes too far"
    {
      fabr_parser *op =
        fabr_n_seq(
          "op",
          fabr_n("exp"),
          fabr_rex("[\+\-\*\/]"),
          fabr_n("exp"),
          NULL);
      fabr_parser *exp =
        //fabr_n_alt("exp", op, val, NULL);
        fabr_n_alt("exp", op, NULL);
      p = exp;

      t = fabr_parse("0", 0, p);

      ensure(t->result == -1);
      ensure(t->note === NULL);

      ensure(fabr_error_message(t) === "too much recursion, parser loop?");
    }
  }

  describe "fabr_parse_all(input, offset, parser)"
  {

    it "is successful when all the input is reached"
    {
      p = fabr_rep(fabr_string("x"), 1, 4);
      t = fabr_parse_all("x", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"rep-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "fails when only part of the input is reached"
    {
      p = fabr_rep(fabr_string("x"), 1, 2);
      t = fabr_parse_all("xxy", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 0, 0, 2, \"not all the input could be parsed\", \"rep-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 1, 1, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }
  }

  describe "fabr_parse_c(input, offset, parser, flags)"
  {
    it "lets parsing proceed if not FABR_F_PRUNE"
    {
      p = fabr_rep(fabr_string("x"), 1, 4);
      t = fabr_parse_f("x", 0, p, FABR_F_ALL);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"rep-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 0, 1, 0, null, \"string-00\", [] ]\n"
        "] ]");
    }
  }

  describe "errors"
  {
    they "propagate down the fabr_tree"
    {
      p = fabr_seq(fabr_n("p0"), fabr_n("p1"), NULL);
      t = fabr_parse_all("x", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, -1, 0, 0, null, \"seq-0\", [\n"
        "  [ \"p0\", -1, 0, 0, \"unlinked fabr_n(\"p0\")\", \"n-00\", [] ]\n"
        "] ]");
    }
  }

  describe "fabr_error_message()"
  {
    it "returns the error message"
    {
      p = fabr_seq(fabr_n("p0"), fabr_n("p1"), NULL);
      t = fabr_parse_all("x", 0, p);

      char *s = fabr_error_message(t);

      // for now, the returned string belongs to the tree

      ensure(s === "unlinked fabr_n(\"p0\")");
    }

    it "returns NULL if there are no errors"
    {
      p = fabr_rep(fabr_string("x"), 1, 4);
      t = fabr_parse_all("x", 0, p);

      char *s = fabr_error_message(t);

      ensure(s == NULL);
    }
  }

  describe "fabr_match()"
  {
    it "returns 0 when the input doesn't match the parser"
    {
      p = fabr_rex("[a-zA-Z][a-zA-Z0-9]*");

      expect(fabr_match("1abc", p) == 0);
    }

    it "returns 1 when the input matches the parser"
    {
      p = fabr_rex("[a-zA-Z][a-zA-Z0-9]*");

      expect(fabr_match("abc1", p) == 1);
    }
  }
}

