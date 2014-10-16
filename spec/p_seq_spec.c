
//
// specifying aabro
//
// Fri Apr 11 06:01:20 JST 2014
//

#include "aabro.h"


context "sequence"
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

  describe "fabr_seq(s)"
  {
    it "creates a sequence parser struct"
    {
      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);

      ensure(p != NULL);
      ensure(p->name == NULL);
      ensure(fabr_p_child(p, 0) != NULL);
      ensure(fabr_p_child(p, 1) != NULL);
      ensure(fabr_p_child(p, 2) == NULL);
    }
  }
  describe "fabr_n_seq(name, s)"
  {
    it "creates a named sequence parser struct"
    {
      p = fabr_n_seq("xtheny", fabr_string("x"), fabr_string("y"), NULL);

      ensure(p != NULL);
      ensure(p->name === "xtheny");
      ensure(fabr_p_child(p, 0) != NULL);
      ensure(fabr_p_child(p, 1) != NULL);
      ensure(fabr_p_child(p, 2) == NULL);
    }
  }

  describe "fabr_parser_to_string(p)"
  {
    it "returns a string representation of the seq parser struct"
    {
      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_seq( /* 0 */\n"
        "  fabr_string(\"x\") /* 00 */,\n"
        "  fabr_string(\"y\") /* 01 */,\n"
        "  NULL)");
    }

    it "returns a string representation of the named seq parser struct"
    {
      p = fabr_n_seq("xtheny", fabr_string("x"), fabr_string("y"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_n_seq( /* 0 */\n"
        "  \"xtheny\",\n"
        "  fabr_string(\"x\") /* 00 */,\n"
        "  fabr_string(\"y\") /* 01 */,\n"
        "  NULL)");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("xy", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, 1, 0, 2, null, \"seq-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 1, 1, 1, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "succeeds (named parser)"
    {
      p = fabr_n_seq("xtheny", fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("xy", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ \"xtheny\", 1, 0, 2, null, \"seq-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 1, 1, 1, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "fails"
    {
      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("z", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, 0, 0, 0, null, \"seq-0\", [] ]");
    }

    it "fails (2nd step)"
    {
      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("xz", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, 0, 0, 0, null, \"seq-0\", [] ]");
    }

    it "reports the failed tries if not FABR_F_PRUNE"
    {
      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse_f("xz", 0, p, FABR_F_ALL);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, 0, 0, 0, null, \"seq-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 0, 1, 0, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "propagates errors"
    {
      p = fabr_seq(fabr_string("x"), fabr_n("y"), NULL);
      t = fabr_parse("xz", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, -1, 0, 0, null, \"seq-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ \"y\", -1, 1, 0, \"unlinked fabr_n(\"y\")\", \"n-01\", [] ]\n"
        "] ]");
    }
  }
}

