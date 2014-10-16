
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
    fabr_tree *t = NULL;
    fabr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) fabr_tree_free(t);
    if (p != NULL) fabr_parser_free(p);
  }

  describe "fabr_n(s)"
  {
    it "creates a placeholder parser struct"
    {
      p = fabr_n("x");

      ensure(p != NULL);
      ensure(p->name === "x");
      ensure(p->string == NULL);
    }
  }

  describe "fabr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct (not linked)"
    {
      p = fabr_n("y");
      char *s = fabr_parser_to_string(p);

      ensure(s ===f "fabr_n(\"y\") /* 0 */ /* not linked */");
    }
    it "returns a string representation of the parser struct (linked)"
    {
      p = fabr_n_alt("p", fabr_string("p"), fabr_n("p"), NULL); // snake
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_n_alt( /* 0 */\n"
        "  \"p\",\n"
        "  fabr_string(\"p\") /* 00 */,\n"
        "  fabr_n(\"p\") /* 01 */,\n"
        "  NULL)");
    }
  }

  context "parsing"
  {
    // When a placeholder is called for parsing, it means the parser tree
    // is incomplete. Do return -1 (error).

    it "returns -1 (error) (not linked)"
    {
      p = fabr_n("z");
      t = fabr_parse("x", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ \"z\", -1, 0, 0, \"unlinked fabr_n(\"z\")\", \"n-0\", [] ]");
    }

    it "parses (linked)"
    {
      fabr_parser *val =
        fabr_n_alt(
          "val",
          fabr_rex("-?[0-9]+"),
          fabr_seq(fabr_string("("), fabr_n("exp"), fabr_string(")"), NULL),
          NULL);
      fabr_parser *op =
        fabr_n_seq(
          "op",
          fabr_n("exp"),
          fabr_rex("[\+\-\*\/]"),
          fabr_n("exp"),
          NULL);
      fabr_parser *exp =
        fabr_n_alt("exp", val, op, NULL);
      p = exp;

      t = fabr_parse("0", 0, p);
      char *s = fabr_tree_to_string(t, NULL, 0);

      ensure(s ===f ""
        "[ \"exp\", 1, 0, 1, null, \"alt-0\", [\n"
        "  [ \"val\", 1, 0, 1, null, \"alt-00\", [\n"
        "    [ null, 1, 0, 1, null, \"rex-000\", [] ]\n"
        "  ] ]\n"
        "] ]");
    }
  }

  context "linking"
  {
    it "resolves fabr_n() when the name is defined"
    {
      p = fabr_n_alt("p", fabr_string("p"), fabr_n("p"), NULL);
        // yes, I know, it's a joke

      fabr_parser *c = fabr_p_child(p, 1);
      ensure(fabr_p_child(c, 0) == p);
    }
  }
}

context "fabr_n_xxx"
{
  before each
  {
    fabr_parser *p = NULL;
  }
  after each
  {
    if (p != NULL) fabr_parser_free(p);
  }

  it "doesn't fall in an infinite loop when resolving names"
  {
    fabr_parser *entry =
      fabr_n_seq(
        "entry",
        fabr_n("value"),
        NULL);

    fabr_parser *entries =
      fabr_n_rep(
        "entries",
        fabr_seq(
          entry,
          fabr_rep(
            entry,
            0, -1),
          NULL
        ),
        0, 1);

    p =
      fabr_n_alt(
        "value",
        fabr_n_rex("number", "-?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?"),
        fabr_n_seq("object", fabr_string("{"), entries, fabr_string("}"), NULL),
        NULL);

    ensure(fabr_parser_to_string(p) ===f ""
      "fabr_n_alt( /* 0 */\n"
      "  \"value\",\n"
      "  fabr_n_rex(\"number\", \"-?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?\") /* 00 */,\n"
      "  fabr_n_seq( /* 01 */\n"
      "    \"object\",\n"
      "    fabr_string(\"{\") /* 010 */,\n"
      "    fabr_n_rep( /* 011 */\n"
      "      \"entries\",\n"
      "      fabr_seq( /* 0110 */\n"
      "        fabr_n_seq( /* 01100 */\n"
      "          \"entry\",\n"
      "          fabr_n(\"value\") /* 011000 */,\n"
      "          NULL),\n"
      "        fabr_rep( /* 01101 */\n"
      "          fabr_n(\"entry\") /* 01100 */, 0, -1),\n"
      "        NULL), 0, 1),\n"
      "    fabr_string(\"}\") /* 012 */,\n"
      "    NULL),\n"
      "  NULL)");
  }
}

