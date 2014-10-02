
//
// specifying aabro
//
// Wed Apr  2 06:25:31 JST 2014
//

#include "aabro.h"


context "repetition"
{
  before each
  {
    fabr_parser *p = NULL;
    fabr_tree *t = NULL;
  }
  after each
  {
    if (p != NULL) fabr_parser_free(p);
    if (t != NULL) fabr_tree_free(t);
  }

  describe "fabr_rep(s)"
  {
    it "creates a rep parser struct"
    {
      p = fabr_rep(fabr_string("xy"), 1, 2);

      ensure(p != NULL);
      ensure(p->name == NULL);
      ensure(p->min == 1);
      ensure(p->max == 2);
      ensure(fabr_p_child(p, 0) != NULL);
      ensure(fabr_p_child(p, 1) == NULL);
    }
  }
  describe "fabr_n_rep(s)"
  {
    it "creates a named rep parser struct"
    {
      p = fabr_n_rep("xandy", fabr_string("xy"), 1, 2);

      ensure(p != NULL);
      ensure(p->name === "xandy");
      ensure(p->min == 1);
      ensure(p->max == 2);
      ensure(fabr_p_child(p, 0) != NULL);
      ensure(fabr_p_child(p, 1) == NULL);
    }
  }

  describe "fabr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = fabr_rep(fabr_string("xy"), 1, 2);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_rep( /* 0 */\n"
        "  fabr_string(\"xy\") /* 00 */, 1, 2)");
    }
    it "returns a string representation of the named parser struct"
    {
      p = fabr_n_rep("xandy", fabr_string("xy"), 1, 2);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_n_rep( /* 0 */\n"
        "  \"xandy\",\n"
        "  fabr_string(\"xy\") /* 00 */, 1, 2)");
    }
  }

  describe "fabr_parser_to_s(p)"
  {
    it "returns a string representation of the parser"
    {
      p = fabr_rep(fabr_string("xy"), 1, 2);
      char *s = fabr_parser_to_s(p);

      ensure(s ===f "rep t1 c1 mn1 mx2");
    }
    it "returns a string representation of the named parser"
    {
      p = fabr_n_rep("xandy", fabr_string("xy"), 1, 2);
      char *s = fabr_parser_to_s(p);

      ensure(s ===f "rep t1 'xandy' c1 mn1 mx2");
    }
  }

  context "parsing"
  {
    it "matches once"
    {
      p = fabr_rep(fabr_string("xy"), 0, 1);
      t = fabr_parse("xy", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 2, null, \"rep-0\", [\n"
        "  [ null, 1, 0, 2, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "matches once (and names)"
    {
      p = fabr_n_rep("xandy", fabr_string("xy"), 0, 1);
      t = fabr_parse("xy", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ \"xandy\", 1, 0, 2, null, \"rep-0\", [\n"
        "  [ null, 1, 0, 2, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "matches twice"
    {
      p = fabr_rep(fabr_string("xy"), 0, 2);
      t = fabr_parse("xyxy", 0, p);

      ensure(t->result == 1);
      ensure(t->offset == 0);
      ensure(t->length == 4);
      ensure(fabr_t_child(t, 0) != NULL);
      ensure(fabr_t_child(t, 1) != NULL);
      ensure(fabr_t_child(t, 0)->result == 1);
      ensure(fabr_t_child(t, 0)->offset == 0);
      ensure(fabr_t_child(t, 0)->length == 2);
      ensure(fabr_t_child(t, 1)->result == 1);
      ensure(fabr_t_child(t, 1)->offset == 2);
      ensure(fabr_t_child(t, 1)->length == 2);

      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 1, 0, 4, null, \"rep-0\", [\n"
        "  [ null, 1, 0, 2, null, \"string-00\", [] ],\n"
        "  [ null, 1, 2, 2, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "doesn't match"
    {
      p = fabr_rep(fabr_string("xy"), 2, 3);
      t = fabr_parse("xy", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 0, 0, 2, null, \"rep-0\", [] ]");
    }

    it "relates all the tries when not FABR_F_PRUNE"
    {
      p = fabr_rep(fabr_string("xy"), 2, 3);
      t = fabr_parse_f("xy", 0, p, FABR_F_ALL);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, 0, 0, 2, null, \"rep-0\", [\n"
        "  [ null, 1, 0, 2, null, \"string-00\", [] ],\n"
        "  [ null, 0, 2, 0, null, \"string-00\", [] ]\n"
        "] ]");
    }

    it "propagates errors"
    {
      p = fabr_rep(fabr_n("x"), 2, 3);
      t = fabr_parse("x", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      ensure(s ===f ""
        "[ null, -1, 0, 0, null, \"rep-0\", [\n"
        "  [ \"x\", -1, 0, 0, \"unlinked fabr_n(\"x\")\", \"n-00\", [] ]\n"
        "] ]");
    }
  }
}

