
//
// specifying aabro
//
// Fri Jul 25 05:48:23 JST 2014
//

#include "aabro.h"


describe "fabr_r(), final child"
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

  describe "fabr_r()"
  {
    it "returns a 'marker' parser"
    {
      p = fabr_r("{4,5}");

      ensure(p->type == 9);
      ensure(p->string === "{4,5}");
    }

    it "wraps its parent"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), fabr_r("*"));
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_rep( /* 0 */\n"
        "  fabr_alt( /* 00 */\n"
        "    fabr_string(\"x\") /* 000 */,\n"
        "    fabr_string(\"y\") /* 001 */,\n"
        "    NULL), 0, -1)");
    }

    it "understands {4,5}"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), fabr_r("{4,5}"));
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_rep( /* 0 */\n"
        "  fabr_alt( /* 00 */\n"
        "    fabr_string(\"x\") /* 000 */,\n"
        "    fabr_string(\"y\") /* 001 */,\n"
        "    NULL), 4, 5)");
    }

    it "understands {4, 5}"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), fabr_r("{4, 5}"));
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_rep( /* 0 */\n"
        "  fabr_alt( /* 00 */\n"
        "    fabr_string(\"x\") /* 000 */,\n"
        "    fabr_string(\"y\") /* 001 */,\n"
        "    NULL), 4, 5)");
    }

    it "understands {3}"
    {
      p = fabr_alt(fabr_string("x"), fabr_string("y"), fabr_r("{3}"));
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_rep( /* 0 */\n"
        "  fabr_alt( /* 00 */\n"
        "    fabr_string(\"x\") /* 000 */,\n"
        "    fabr_string(\"y\") /* 001 */,\n"
        "    NULL), 3, 3)");
    }

    it "steals the parent name"
    {
      p = fabr_n_alt("n", fabr_string("x"), fabr_string("y"), fabr_r("{4, 5}"));
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_n_rep( /* 0 */\n"
        "  \"n\",\n"
        "  fabr_alt( /* 00 */\n"
        "    fabr_string(\"x\") /* 000 */,\n"
        "    fabr_string(\"y\") /* 001 */,\n"
        "    NULL), 4, 5)");
    }
  }

  describe "fabr_n_r()"
  {
    it "keeps its own name"
    {
      p = fabr_n_alt("an", fabr_string("x"), fabr_string("y"), fabr_n_r("rn", "*"));
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_n_rep( /* 0 */\n"
        "  \"rn\",\n"
        "  fabr_n_alt( /* 00 */\n"
        "    \"an\",\n"
        "    fabr_string(\"x\") /* 000 */,\n"
        "    fabr_string(\"y\") /* 001 */,\n"
        "    NULL), 0, -1)");
    }

    it "keeps its own name (2)"
    {
      p = fabr_n_alt("an", fabr_string("x"), fabr_string("y"), fabr_n_r("", "*"));
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_n_rep( /* 0 */\n"
        "  \"\",\n"
        "  fabr_n_alt( /* 00 */\n"
        "    \"an\",\n"
        "    fabr_string(\"x\") /* 000 */,\n"
        "    fabr_string(\"y\") /* 001 */,\n"
        "    NULL), 0, -1)");
    }
  }
}

