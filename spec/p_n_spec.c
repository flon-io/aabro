
//
// specifying aabro
//
// Mon Apr 28 06:38:05 JST 2014
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

  describe "abr_n(s)"
  {
    it "creates a placeholder parser struct"
    {
      p = abr_n("x");

      ensure(p != NULL);
      ensure(p->name === "x");
      ensure(p->string == NULL);
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct (not linked)"
    {
      p = abr_n("y");
      char *s = abr_parser_to_string(p);

      ensure(s ===f "abr_n(\"y\") /* not linked */");
    }
    it "returns a string representation of the parser struct (linked)"
    {
      p = abr_n_alt("p", abr_string("p"), abr_n("p"), NULL); // snake
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_n_alt(\n"
        "  \"p\",\n"
        "  abr_string(\"p\"),\n"
        "  abr_n(\"p\"),\n"
        "  NULL)");
    }
  }

  context "parsing"
  {
    // When a placeholder is called for parsing, it means the parser tree
    // is incomplete. Do fail.

    // TODO: return -1 (error) instead of 0 (failure)
    //
    // error when unliked
    // else delegate to linked parser...

    it "returns an error (not linked)"
    {
      p = abr_n("z");
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f "[ \"z\", -1, 0, -1, \"n\", [] ]");
    }

    it "parses (linked)"
  }

  context "linking"
  {
    it "resolves abr_n() when the name is defined"
    {
      p = abr_n_alt("p", abr_string("p"), abr_n("p"), NULL);
        // yes, I know, it's a joke

      ensure(p->children[1]->children[0] == p);
      ensure(p->children[1]->children[1] == NULL);
    }
  }
}

