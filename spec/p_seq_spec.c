
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
    abr_tree *t = NULL;
    abr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
    if (p != NULL) abr_parser_free(p);
  }

  describe "abr_seq(s)"
  {
    it "creates a sequence parser struct"
    {
      p = abr_seq(abr_string("x"), abr_string("y"), NULL);

      ensure(p != NULL);
      ensure(p->name == NULL);
      ensure(abr_p_child(p, 0) != NULL);
      ensure(abr_p_child(p, 1) != NULL);
      ensure(abr_p_child(p, 2) == NULL);
    }
  }
  describe "abr_n_seq(name, s)"
  {
    it "creates a named sequence parser struct"
    {
      p = abr_n_seq("xtheny", abr_string("x"), abr_string("y"), NULL);

      ensure(p != NULL);
      ensure(p->name === "xtheny");
      ensure(abr_p_child(p, 0) != NULL);
      ensure(abr_p_child(p, 1) != NULL);
      ensure(abr_p_child(p, 2) == NULL);
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the seq parser struct"
    {
      p = abr_seq(abr_string("x"), abr_string("y"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_seq(\n"
        "  abr_string(\"x\"),\n"
        "  abr_string(\"y\"),\n"
        "  NULL)");
    }

    it "returns a string representation of the named seq parser struct"
    {
      p = abr_n_seq("xtheny", abr_string("x"), abr_string("y"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_n_seq(\n"
        "  \"xtheny\",\n"
        "  abr_string(\"x\"),\n"
        "  abr_string(\"y\"),\n"
        "  NULL)");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = abr_seq(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("xy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 2, null, \"seq-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 1, 1, 1, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "succeeds (named parser)"
    {
      p = abr_n_seq("xtheny", abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("xy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ \"xtheny\", 1, 0, 2, null, \"seq-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 1, 1, 1, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "fails"
    {
      p = abr_seq(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("z", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 0, 0, 0, null, \"seq-0\", [] ]");
    }

    it "fails (2nd step)"
    {
      p = abr_seq(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("xz", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 0, 0, 0, null, \"seq-0\", [] ]");
    }

    it "reports the failed tries if not ABR_F_PRUNE"
    {
      p = abr_seq(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse_f("xz", 0, p, ABR_F_ALL);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 0, 0, 0, null, \"seq-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ null, 0, 1, 0, null, \"string-01\", [] ]\n"
        "] ]");
    }

    it "propagates errors"
    {
      p = abr_seq(abr_string("x"), abr_n("y"), NULL);
      t = abr_parse("xz", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, -1, 0, 0, null, \"seq-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
        "  [ \"y\", -1, 1, 0, \"unlinked abr_n(\"y\")\", \"n-01\", [] ]\n"
        "] ]");
    }
  }
}

