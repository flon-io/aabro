
//
// specifying aabro
//
// Fri Jul 25 05:48:23 JST 2014
//

#include "aabro.h"


describe "abr_r(), final child"
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

  describe "abr_r()"
  {
    it "returns a 'marker' parser"
    {
      p = abr_r("{4,5}");

      ensure(p->type == 10);
      ensure(p->string === "{4,5}");
    }

    it "wraps its parent"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), abr_r("*"));
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_rep(\n"
        "  abr_alt(\n"
        "    abr_string(\"x\"),\n"
        "    abr_string(\"y\"),\n"
        "    NULL), 0, -1)");
    }

    it "understands {4,5}"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), abr_r("{4,5}"));
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_rep(\n"
        "  abr_alt(\n"
        "    abr_string(\"x\"),\n"
        "    abr_string(\"y\"),\n"
        "    NULL), 4, 5)");
    }

    it "understands {4, 5}"
    {
      p = abr_alt(abr_string("x"), abr_string("y"), abr_r("{4, 5}"));
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_rep(\n"
        "  abr_alt(\n"
        "    abr_string(\"x\"),\n"
        "    abr_string(\"y\"),\n"
        "    NULL), 4, 5)");
    }

    it "steals the parent name"
    {
      p = abr_n_alt("n", abr_string("x"), abr_string("y"), abr_r("{4, 5}"));
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_n_rep(\n"
        "  \"n\",\n"
        "  abr_alt(\n"
        "    abr_string(\"x\"),\n"
        "    abr_string(\"y\"),\n"
        "    NULL), 4, 5)");
    }
  }
}

