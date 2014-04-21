
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
      ensure(p->children[0] != NULL);
      ensure(p->children[1] != NULL);
      ensure(p->children[2] == NULL);
    }
  }
  describe "abr_n_alt(s, p0, p1, ...)"
  {
    it "creates a named alternative parser struct"
    {
      p = abr_n_alt("letter", abr_string("x"), abr_string("y"), NULL);

      ensure(p != NULL);
      ensure(p->name === "letter");
      ensure(p->children[0] != NULL);
      ensure(p->children[1] != NULL);
      ensure(p->children[2] == NULL);
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_alt(\n"
        "  abr_string(\"x\"),\n"
        "  abr_string(\"y\"),\n"
        "  NULL)");
    }
    it "returns a string representation of the named parser struct"
    {
      p = abr_n_alt("xory", abr_string("x"), abr_string("y"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_n_alt(\n"
        "  \"xory\",\n"
        "  abr_string(\"x\"),\n"
        "  abr_string(\"y\"),\n"
        "  NULL)");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 1, \"alt\", [\n"
        "  [ null, 1, 0, 1, \"string\", [] ]\n"
        "] ]");
    }
    it "succeeds (named parser)"
    {
      p = abr_n_alt("xory", abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ \"xory\", 1, 0, 1, \"alt\", [\n"
        "  [ null, 1, 0, 1, \"string\", [] ]\n"
        "] ]");
    }

    it "succeeds (2nd alt)"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("y", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 1, \"alt\", [\n"
        "  [ null, 0, 0, -1, \"string\", [] ],\n"
        "  [ null, 1, 0, 1, \"string\", [] ]\n"
        "] ]");
    }

    it "fails"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("z", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 0, 0, -1, \"alt\", [\n"
        "  [ null, 0, 0, -1, \"string\", [] ],\n"
        "  [ null, 0, 0, -1, \"string\", [] ]\n"
        "] ]");
    }
    it "fails (named parser)"
    {
      p = abr_n_alt("xory", abr_string("x"), abr_string("y"), NULL);
      t = abr_parse("z", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ \"xory\", 0, 0, -1, \"alt\", [\n"
        "  [ null, 0, 0, -1, \"string\", [] ],\n"
        "  [ null, 0, 0, -1, \"string\", [] ]\n"
        "] ]");
    }
  }
}
