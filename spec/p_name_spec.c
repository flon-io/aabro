
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
    fabr_tree *t = NULL;
    fabr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) fabr_tree_free(t);
    if (p != NULL) fabr_parser_free(p);
  }

  describe "fabr_name(s)"
  {
    it "creates a name parser struct"
    {
      p = fabr_name("x", fabr_string("x"));

      ensure(p != NULL);
      ensure(p->name === "x");
      ensure(p->string == NULL);
    }
  }

  describe "fabr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = fabr_name("y", fabr_string("x"));
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_name( /* 0 */\n"
        "  \"y\",\n"
        "  fabr_string(\"x\") /* 00 */)");
    }
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = fabr_name("y", fabr_string("x"));
      t = fabr_parse("x", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ \"y\", 1, 0, 1, null, \"name-0\", [\n"
        "  [ null, 1, 0, 1, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "fails"
    {
      p = fabr_name("y", fabr_string("x"));
      t = fabr_parse("z", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ \"y\", 0, 0, 0, null, \"name-0\", [] ]");
    }
  }
}

