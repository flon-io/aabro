
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
    // is incomplete. Do return -1 (error).

    it "returns -1 (error) (not linked)"
    {
      p = abr_n("z");
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ \"z\", -1, 0, 0, \"unlinked abr_n(\"z\")\", \"n\", [] ]");
    }

    it "parses (linked)"
    {
      abr_parser *val =
        abr_n_alt(
          "val",
          abr_regex("^-?[0-9]+"),
          abr_seq(abr_string("("), abr_n("exp"), abr_string(")"), NULL),
          NULL);
      abr_parser *op =
        abr_n_seq(
          "op",
          abr_n("exp"),
          abr_regex("^[\+\-\*\/]"),
          abr_n("exp"),
          NULL);
      abr_parser *exp =
        abr_n_alt("exp", val, op, NULL);
      p = exp;

      t = abr_parse("0", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ \"exp\", 1, 0, 1, null, \"alt\", [\n"
        "  [ \"val\", 1, 0, 1, null, \"alt\", [\n"
        "    [ null, 1, 0, 1, null, \"regex\", [] ]\n"
        "  ] ]\n"
        "] ]");
    }
  }

  context "linking"
  {
    it "resolves abr_n() when the name is defined"
    {
      p = abr_n_alt("p", abr_string("p"), abr_n("p"), NULL);
        // yes, I know, it's a joke

      abr_parser *c = abr_p_child(p, 1);
      ensure(abr_p_child(c, 0) == p);
    }
  }
}

context "abr_n_xxx"
{
  before each
  {
    abr_parser *p = NULL;
  }
  after each
  {
    if (p != NULL) abr_parser_free(p);
  }

  it "doesn't fall in an infinite loop when resolving names"
  {
    ensure(0 == 1); // break visibly...

    abr_parser *entry =
      abr_n_seq(
        "entry",
        //abr_n_regex("symbol", "^[a-zA-Z_]+"),
        //abr_string(":"),
        abr_n("value"),
        NULL);

    abr_parser *entries =
      abr_n_rep(
        "entries",
        abr_seq(
          entry,
          abr_rep(
            //abr_seq(abr_string(","), entry, NULL),
            entry,
            0, -1),
          NULL
        ),
        0, 1);

    p =
      abr_n_alt(
        "value",
        abr_n_regex("number", "^-?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?"),
        abr_n_seq("object", abr_string("{"), entries, abr_string("}"), NULL),
        NULL);

    ensure(abr_parser_to_string(p) ===f ""
      "nada");
  }
}

