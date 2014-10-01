
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
    fabr_tree *t = NULL;
    fabr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) fabr_tree_free(t);
    if (p != NULL) fabr_parser_free(p);
  }

  describe "fabr_string(s)"
  {
    it "creates a string parser struct"
    {
      p = fabr_string("beowulf");

      ensure(p != NULL);
      ensure(p->name == NULL);
      ensure(p->string === "beowulf");
    }
  }
  describe "fabr_n_string(name, s)"
  {
    it "creates a named string parser struct"
    {
      p = fabr_n_string("bf", "beowulf");

      ensure(p != NULL);
      ensure(p->name === "bf");
      ensure(p->string === "beowulf");
    }
  }

  describe "fabr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = fabr_string("toto");
      char *s = fabr_parser_to_string(p);

      ensure(s ===f "fabr_string(\"toto\") /* 0 */");
    }

    it "returns a string representation of the named parser struct"
    {
      p = fabr_n_string("to", "toto");
      char *s = fabr_parser_to_string(p);

      ensure(s ===f "fabr_n_string(\"to\", \"toto\") /* 0 */");
    }
  }

  describe "fabr_parser_to_s(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = fabr_string("toto");
      char *s = fabr_parser_to_s(p);

      ensure(s ===f "string t0 \"toto\" c0");
    }

    it "returns a string representation of the named parser struct"
    {
      p = fabr_n_string("to", "toto");
      char *s = fabr_parser_to_s(p);

      ensure(s ===f "string t0 'to' \"toto\" c0");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = fabr_string("xxx");
      t = fabr_parse("xxx", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f "[ null, 1, 0, 3, null, \"string-0\", [] ]");
    }

    it "succeeds (named parser)"
    {
      p = fabr_n_string("3x", "xxx");
      t = fabr_parse("xxx", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f "[ \"3x\", 1, 0, 3, null, \"string-0\", [] ]");
    }

    it "fails"
    {
      p = fabr_string("xxx");
      t = fabr_parse("yyy", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f "[ null, 0, 0, 0, null, \"string-0\", [] ]");
    }
  }
}

