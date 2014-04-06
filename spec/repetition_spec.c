
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
      ensure(p->min == 1);
      ensure(p->max == 2);
      ensure(p->children != NULL);
      ensure(p->children[0] != NULL);
      ensure(p->children[1] == NULL);
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_rep(abr_string("xy"), 1, 2);
      char *s = abr_parser_to_string(p);

      ensure(s ===f "abr_rep(\n"
        "  abr_string(\"xy\"),\n"
        "  1, 2)");
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
        "[ (null), 1, 0, 2, [\n"
        "  [ (null), 1, 0, 2, [] ]\n"
        "] ]");
    }

    it "matches twice"
    {
      p = abr_rep(abr_string("xy"), 0, 2);
      t = abr_parse("xyxy", 0, p);

      ensure(t->success == 1);
      ensure(t->offset == 0);
      ensure(t->length == 4);
      ensure(t->children[0] != NULL);
      ensure(t->children[1] != NULL);
      ensure(t->children[2] == NULL);
      ensure(t->children[0]->success == 1);
      ensure(t->children[0]->offset == 0);
      ensure(t->children[0]->length == 2);
      ensure(t->children[1]->success == 1);
      ensure(t->children[1]->offset == 2);
      ensure(t->children[1]->length == 2);

      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ (null), 1, 0, 4, [\n"
        "  [ (null), 1, 0, 2, [] ]\n"
        "  [ (null), 1, 2, 2, [] ]\n"
        "] ]");
    }

    it "doesn't match"
    {
      p = abr_rep(abr_string("xy"), 2, 3);
      t = abr_parse("xy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f ""
        "[ (null), 0, 0, 2, [\n"
        "  [ (null), 1, 0, 2, [] ]\n"
        "] ]");
    }
  }
}

