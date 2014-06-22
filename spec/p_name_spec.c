
//
// specifying aabro
//
// Wed Apr  9 06:26:04 JST 2014
//

#include "aabro.h"


context "name"
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

  describe "abr_name(s)"
  {
    it "creates a name parser struct"
    {
      p = abr_name("x", abr_string("x"));

      ensure(p != NULL);
      ensure(p->name === "x");
      ensure(p->string == NULL);
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_name("y", abr_string("x"));
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_name(\n"
        "  \"y\",\n"
        "  abr_string(\"x\"))");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = abr_name("y", abr_string("x"));
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ \"y\", 1, 0, 1, null, \"name\", [\n"
        "  [ null, 1, 0, 1, null, \"string\", [] ]\n"
        "] ]");
    }

    it "fails"
    {
      p = abr_name("y", abr_string("x"));
      t = abr_parse("z", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ \"y\", 0, 0, 0, null, \"name\", [\n"
        "  [ null, 0, 0, 0, null, \"string\", [] ]\n"
        "] ]");
    }
  }
}

