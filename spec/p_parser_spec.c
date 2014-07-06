
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

  describe "abr_parse(input, offset, parser)"
  {
    it "parses as much as it can"
    {
      p = abr_rep(abr_string("x"), 1, 4);
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"rep\", [\n"
        "  [ null, 1, 0, 1, null, \"string\", [] ]\n"
        "] ]");
    }

    it "is successful even if it didn't parse all the input"
    {
      p = abr_rep(abr_string("x"), 1, 2);
      t = abr_parse("xxy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 2, null, \"rep\", [\n"
        "  [ null, 1, 0, 1, null, \"string\", [] ],\n"
        "  [ null, 1, 1, 1, null, \"string\", [] ]\n"
        "] ]");
    }

    it "returns an error when recursion goes too far"
    {
      //abr_parser *val =
      //  abr_n_alt(
      //    "val",
      //    abr_regex("^-?[0-9]+"),
      //    abr_seq(abr_string("("), abr_n("exp"), abr_string(")"), NULL),
      //    NULL);
      abr_parser *op =
        abr_n_seq(
          "op",
          abr_n("exp"),
          abr_regex("^[\+\-\*\/]"),
          abr_n("exp"),
          NULL);
      abr_parser *exp =
        //abr_n_alt("exp", op, val, NULL);
        abr_n_alt("exp", op, NULL);
      p = exp;

      t = abr_parse("0", 0, p);

      ensure(t->result == -1);
      ensure(t->note === NULL);

      // TODO: look up error
    }
  }

  describe "abr_parse_all(input, offset, parser)"
  {

    it "is successful when all the input is reached"
    {
      p = abr_rep(abr_string("x"), 1, 4);
      t = abr_parse_all("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"rep\", [\n"
        "  [ null, 1, 0, 1, null, \"string\", [] ]\n"
        "] ]");
    }

    it "fails when only part of the input is reached"
    {
      p = abr_rep(abr_string("x"), 1, 2);
      t = abr_parse_all("xxy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 0, 0, 2, \"not all the input could be parsed\", \"rep\", [\n"
        "  [ null, 1, 0, 1, null, \"string\", [] ],\n"
        "  [ null, 1, 1, 1, null, \"string\", [] ]\n"
        "] ]");
    }
  }

  describe "abr_parse_c(input, offset, parser, conf)"
  {
    it "lets parsing proceed with conf.prune = 0"
    {
      abr_conf c = { .all = 1, .prune = 0 };
      p = abr_rep(abr_string("x"), 1, 4);
      t = abr_parse_c("x", 0, p, c);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"rep\", [\n"
        "  [ null, 1, 0, 1, null, \"string\", [] ],\n"
        "  [ null, 0, 1, 0, null, \"string\", [] ]\n"
        "] ]");
    }
  }

  describe "errors"
  {
    they "propagate down the abr_tree"
    {
      p = abr_seq(abr_n("p0"), abr_n("p1"), NULL);
      t = abr_parse_all("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, -1, 0, 0, null, \"seq\", [\n"
        "  [ \"p0\", -1, 0, 0, \"unlinked abr_n(\"p0\")\", \"n\", [] ]\n"
        "] ]");
    }
  }

  describe "abr_error_message()"
  {
    it "returns the error message"
    {
      p = abr_seq(abr_n("p0"), abr_n("p1"), NULL);
      t = abr_parse_all("x", 0, p);

      char *s = abr_error_message(t);

      // for now, the returned string belongs to the tree

      ensure(s === "unlinked abr_n(\"p0\")");
    }

    it "returns NULL if there are no errors"
    {
      p = abr_rep(abr_string("x"), 1, 4);
      t = abr_parse_all("x", 0, p);

      char *s = abr_error_message(t);

      ensure(s == NULL);
    }
  }
}

