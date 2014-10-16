
//
// specifying aabro
//
// Thu Apr 10 05:47:54 JST 2014
//

#include "aabro.h"


context "alternative"
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

  describe "fabr_alt(p0, p1, ...)"
  {
    it "creates an alternative parser struct"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), NULL);

      ensure(p != NULL);
      ensure(fabr_p_child(p, 0) != NULL);
      ensure(fabr_p_child(p, 1) != NULL);
      ensure(fabr_p_child(p, 2) == NULL);
    }
  }
  describe "fabr_n_alt(s, p0, p1, ...)"
  {
    it "creates a named alternative parser struct"
    {
      p = fabr_n_alt("letter", fabr_string("x"), fabr_string("y"), NULL);

      ensure(p != NULL);
      ensure(p->name === "letter");
      ensure(fabr_p_child(p, 0) != NULL);
      ensure(fabr_p_child(p, 1) != NULL);
      ensure(fabr_p_child(p, 2) == NULL);
    }
  }

  describe "fabr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_alt( /* 0 */\n"
        "  fabr_string(\"x\") /* 00 */,\n"
        "  fabr_string(\"y\") /* 01 */,\n"
        "  NULL)");
    }
    it "returns a string representation of the named parser struct"
    {
      p = fabr_n_alt("xory", fabr_string("x"), fabr_string("y"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_n_alt( /* 0 */\n"
        "  \"xory\",\n"
        "  fabr_string(\"x\") /* 00 */,\n"
        "  fabr_string(\"y\") /* 01 */,\n"
        "  NULL)");
    }
  }

  describe "fabr_parser_to_s(p)"
  {
    it "returns a string representation of the parser"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), NULL);
      char *s = fabr_parser_to_s(p);

      ensure(s ===f "alt t2 c2");
    }
    it "returns a string representation of the named parser"
    {
      p = fabr_n_alt("xory", fabr_string("x"), fabr_string("y"), NULL);
      char *s = fabr_parser_to_s(p);

      ensure(s ===f "alt t2 'xory' c2");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("x", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"alt-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "succeeds (named parser)"
    {
      p = fabr_n_alt("xory", fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("x", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ \"xory\", 1, 0, 1, null, \"alt-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "succeeds (2nd alt)"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("y", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"alt-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "fails"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("z", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, 0, 0, 0, null, \"alt-0\", [] ]");
    }

    it "fails (named parser)"
    {
      p = fabr_n_alt("xory", fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse("z", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ \"xory\", 0, 0, 0, null, \"alt-0\", [] ]");
    }

    it "reports the failed attempt if not FABR_F_PRUNE"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), NULL);
      t = fabr_parse_f("y", 0, p, FABR_F_ALL);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"alt-0\", [\n"
        "  [ null, 0, 0, 0, null, \"string-00\", [] ],\n"
        "  [ null, 1, 0, 1, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "propagates errors"
    {
      p = fabr_alt(fabr_string("x"), fabr_n("y"), NULL);
      t = fabr_parse("z", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ null, -1, 0, 0, null, \"alt-0\", [\n"
        "  [ \"y\", -1, 0, 0, \"unlinked fabr_n(\"y\")\", \"n-01\", [] ]\n"
        "] ]");
    }
  }
}

