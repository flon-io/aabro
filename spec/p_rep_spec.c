
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
    abr_parser *p = NULL;
    abr_tree *t = NULL;
  }
  after each
  {
    if (p != NULL) abr_parser_free(p);
    if (t != NULL) abr_tree_free(t);
  }

  describe "abr_rep(s)"
  {
    it "creates a rep parser struct"
    {
      p = abr_rep(abr_string("xy"), 1, 2);

      ensure(p != NULL);
      ensure(p->name == NULL);
      ensure(p->min == 1);
      ensure(p->max == 2);
      ensure(abr_p_child(p, 0) != NULL);
      ensure(abr_p_child(p, 1) == NULL);
    }
  }
  describe "abr_n_rep(s)"
  {
    it "creates a named rep parser struct"
    {
      p = abr_n_rep("xandy", abr_string("xy"), 1, 2);

      ensure(p != NULL);
      ensure(p->name === "xandy");
      ensure(p->min == 1);
      ensure(p->max == 2);
      ensure(abr_p_child(p, 0) != NULL);
      ensure(abr_p_child(p, 1) == NULL);
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_rep(abr_string("xy"), 1, 2);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_rep(\n"
        "  abr_string(\"xy\"), 1, 2)");
    }
    it "returns a string representation of the named parser struct"
    {
      p = abr_n_rep("xandy", abr_string("xy"), 1, 2);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_n_rep(\n"
        "  \"xandy\",\n"
        "  abr_string(\"xy\"), 1, 2)");
    }
  }

  describe "abr_parser_to_s(p)"
  {
    it "returns a string representation of the parser"
    {
      p = abr_rep(abr_string("xy"), 1, 2);
      char *s = abr_parser_to_s(p);

      ensure(s ===f "rep t2 c1 mn1 mx2");
    }
    it "returns a string representation of the named parser"
    {
      p = abr_n_rep("xandy", abr_string("xy"), 1, 2);
      char *s = abr_parser_to_s(p);

      ensure(s ===f "rep t2 'xandy' c1 mn1 mx2");
    }
  }

  context "parsing"
  {
    it "matches once"
    {
      p = abr_rep(abr_string("xy"), 0, 1);
      t = abr_parse("xy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 2, null, \"rep\", [\n"
        "  [ null, 1, 0, 2, null, \"string\", [] ]\n"
        "] ]");
    }

    it "matches once (and names)"
    {
      p = abr_n_rep("xandy", abr_string("xy"), 0, 1);
      t = abr_parse("xy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ \"xandy\", 1, 0, 2, null, \"rep\", [\n"
        "  [ null, 1, 0, 2, null, \"string\", [] ]\n"
        "] ]");
    }

    it "matches twice"
    {
      p = abr_rep(abr_string("xy"), 0, 2);
      t = abr_parse("xyxy", 0, p);

      ensure(t->result == 1);
      ensure(t->offset == 0);
      ensure(t->length == 4);
      ensure(abr_t_child(t, 0) != NULL);
      ensure(abr_t_child(t, 1) != NULL);
      ensure(abr_t_child(t, 0)->result == 1);
      ensure(abr_t_child(t, 0)->offset == 0);
      ensure(abr_t_child(t, 0)->length == 2);
      ensure(abr_t_child(t, 1)->result == 1);
      ensure(abr_t_child(t, 1)->offset == 2);
      ensure(abr_t_child(t, 1)->length == 2);

      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 1, 0, 4, null, \"rep\", [\n"
        "  [ null, 1, 0, 2, null, \"string\", [] ],\n"
        "  [ null, 1, 2, 2, null, \"string\", [] ]\n"
        "] ]");
    }

    it "doesn't match"
    {
      p = abr_rep(abr_string("xy"), 2, 3);
      t = abr_parse("xy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 0, 0, 2, null, \"rep\", [] ]");
    }

    it "relates all the tries when not ABR_F_PRUNE"
    {
      p = abr_rep(abr_string("xy"), 2, 3);
      t = abr_parse_f("xy", 0, p, ABR_F_ALL);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, 0, 0, 2, null, \"rep\", [\n"
        "  [ null, 1, 0, 2, null, \"string\", [] ],\n"
        "  [ null, 0, 2, 0, null, \"string\", [] ]\n"
        "] ]");
    }

    it "propagates errors"
    {
      p = abr_rep(abr_n("x"), 2, 3);
      t = abr_parse("x", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ null, -1, 0, 0, null, \"rep\", [\n"
        "  [ \"x\", -1, 0, 0, \"unlinked abr_n(\"x\")\", \"n\", [] ]\n"
        "] ]");
    }
  }
}

