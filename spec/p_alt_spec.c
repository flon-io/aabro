
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
    abr_tree *t = NULL;
    abr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
    if (p != NULL) abr_parser_free(p);
  }

  describe "abr_alt(p0, p1, ...)"
  {
    it "creates an alternative parser struct"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);

      ensure(p != NULL);
      ensure(abr_p_child(p, 0) != NULL);
      ensure(abr_p_child(p, 1) != NULL);
      ensure(abr_p_child(p, 2) == NULL);
    }
  }
  describe "abr_n_alt(s, p0, p1, ...)"
  {
    it "creates a named alternative parser struct"
    {
      p = abr_n_alt("letter", abr_string("x"), abr_string("y"), NULL);

      ensure(p != NULL);
      ensure(p->name === "letter");
      ensure(abr_p_child(p, 0) != NULL);
      ensure(abr_p_child(p, 1) != NULL);
      ensure(abr_p_child(p, 2) == NULL);
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_alt( /* 0 */\n"
        "  abr_string(\"x\") /* 00 */,\n"
        "  abr_string(\"y\") /* 01 */,\n"
        "  NULL)");
    }
    it "returns a string representation of the named parser struct"
    {
      p = abr_n_alt("xory", abr_string("x"), abr_string("y"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_n_alt( /* 0 */\n"
        "  \"xory\",\n"
        "  abr_string(\"x\") /* 00 */,\n"
        "  abr_string(\"y\") /* 01 */,\n"
        "  NULL)");
    }
  }

  describe "abr_parser_to_s(p)"
  {
    it "returns a string representation of the parser"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      char *s = abr_parser_to_s(p);

      ensure(s ===f "alt t2 c2");
    }
    it "returns a string representation of the named parser"
    {
      p = abr_n_alt("xory", abr_string("x"), abr_string("y"), NULL);
      char *s = abr_parser_to_s(p);

      ensure(s ===f "alt t2 'xory' c2");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"alt-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "succeeds (named parser)"
    {
      p = abr_n_alt("xory", abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ \"xory\", 1, 0, 1, null, \"alt-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "succeeds (2nd alt)"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("y", 0, p);
      char *s = abr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"alt-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "fails"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("z", 0, p);
      char *s = abr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 0, 0, 0, null, \"alt-0\", [] ]");
    }

    it "fails (named parser)"
    {
      p = abr_n_alt("xory", abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("z", 0, p);
      char *s = abr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ \"xory\", 0, 0, 0, null, \"alt-0\", [] ]");
    }

    it "reports the failed attempt if not ABR_F_PRUNE"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse_f("y", 0, p, ABR_F_ALL);
      char *s = abr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 1, null, \"alt-0\", [\n"
        "  [ null, 0, 0, 0, null, \"string-00\", [] ],\n"
        "  [ null, 1, 0, 1, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "propagates errors"
    {
      p = abr_alt(abr_string("x"), abr_n("y"), NULL);
      t = abr_parse("z", 0, p);
      char *s = abr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, -1, 0, 0, null, \"alt-0\", [\n"
        "  [ \"y\", -1, 0, 0, \"unlinked abr_n(\"y\")\", \"n-01\", [] ]\n"
        "] ]");
    }
  }
}

