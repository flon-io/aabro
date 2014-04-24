
//
// specifying aabro
//
// Tue Dec 17 22:56:18 JST 2013
//

#include "aabro.h"


context "strings"
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

  describe "abr_string(s)"
  {
    it "creates a string parser struct"
    {
      p = abr_string("beowulf");

      ensure(p != NULL);
      ensure(p->name == NULL);
      ensure(p->string === "beowulf");
    }
  }
  describe "abr_n_string(name, s)"
  {
    it "creates a named string parser struct"
    {
      p = abr_n_string("bf", "beowulf");

      ensure(p != NULL);
      ensure(p->name === "bf");
      ensure(p->string === "beowulf");
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_string("toto");
      char *s = abr_parser_to_string(p);

      ensure(s ===f "abr_string(\"toto\")");
    }

    it "returns a string representation of the named parser struct"
    {
      p = abr_n_string("to", "toto");
      char *s = abr_parser_to_string(p);

      ensure(s ===f "abr_n_string(\"to\", \"toto\")");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = abr_string("xxx");
      t = abr_parse("xxx", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f "[ null, 1, 0, 3, \"string\", [] ]");
    }

    it "succeeds (named parser)"
    {
      p = abr_n_string("3x", "xxx");
      t = abr_parse("xxx", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f "[ \"3x\", 1, 0, 3, \"string\", [] ]");
    }

    it "fails"
    {
      p = abr_string("xxx");
      t = abr_parse("yyy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f "[ null, 0, 0, -1, \"string\", [] ]");
    }
  }
}

