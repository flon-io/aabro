
//
// specifying aabro
//
// Thu Apr 10 05:47:54 JST 2014
//

#include "aabro.h"


describe "fabr_altg()"
{
  before each
  {
    fabr_input i = { "nada", 0, 0 };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  static fabr_tree *_1x(fabr_input *i) { return fabr_str("1x", i, "x"); }
  static fabr_tree *_2x(fabr_input *i) { return fabr_str("2x", i, "xx"); }
  static fabr_tree *_x2(fabr_input *i) { return fabr_str("x2", i, "xx"); }
  static fabr_tree *_3x(fabr_input *i) { return fabr_str("3x", i, "xxx"); }

  static fabr_tree *_xerr(fabr_input *i)
  {
    fabr_tree *r = calloc(1, sizeof(fabr_tree));
    r->result = -1; r->parter = "_xerr"; r->offset = i->offset;

    return r;
  }

  context "greedy=0"
  {
    it "takes the first succesful result"
    {
      i.string = "xx";
      t = fabr_altgr("g", &i, 0, _1x, _2x, NULL);

      expect(i.offset zu== 1);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 1, null, \"alt\", 0, [\n"
        "  [ \"1x\", 1, 0, 1, null, \"str\", 1, \"x\" ]\n"
        "] ]");
    }
  }

  context "greedy=1"
  {
    it "favours the longest successful result"
    {
      i.string = "xx";
      t = fabr_altg("g", &i, _1x, _2x, NULL);

      expect(i.offset zu== 2);

      //fabr_puts(t, i.string, 3);
      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 2, null, \"altg\", 0, [\n"
        "  [ \"1x\", 0, 0, 0, null, \"str\", 1, [] ],\n"
        "  [ \"2x\", 1, 0, 2, null, \"str\", 2, \"xx\" ]\n"
        "] ]");
    }

    it "drops the short winners when pruning (0)"
    {
      i.string = "xx";
      i.flags = FABR_F_PRUNE;
      t = fabr_altg("g", &i, _1x, _2x, NULL);

      expect(i.offset zu== 2);

      //fabr_puts(t, i.string, 3);
      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 2, null, \"altg\", 0, [\n"
        "  [ \"2x\", 1, 0, 2, null, \"str\", 2, \"xx\" ]\n"
        "] ]");
    }

    it "drops the short winners when pruning (1)"
    {
      i.string = "xx";
      i.flags = FABR_F_PRUNE;
      t = fabr_altg("g", &i, _2x, _x2, NULL);

      expect(i.offset zu== 2);

      //fabr_puts(t, i.string, 3);
      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 2, null, \"altg\", 0, [\n"
        "  [ \"x2\", 1, 0, 2, null, \"str\", 2, \"xx\" ]\n"
        "] ]");
    }

    it "drops the short winners when pruning (2)"
    {
      i.string = "xx";
      i.flags = FABR_F_PRUNE;
      t = fabr_altg("g", &i, _2x, _x2, _3x, NULL);

      expect(i.offset zu== 2);

      //fabr_puts(t, i.string, 3);
      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 2, null, \"altg\", 0, [\n"
        "  [ \"x2\", 1, 0, 2, null, \"str\", 2, \"xx\" ]\n"
        "] ]");
    }

    it "drops the short winners when pruning (3)"
    {
      i.string = "xx";
      i.flags = FABR_F_PRUNE;
      t = fabr_altg("g", &i, _2x, _1x, NULL);

      expect(i.offset zu== 2);

      //fabr_puts(t, i.string, 3);
      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 2, null, \"altg\", 0, [\n"
        "  [ \"2x\", 1, 0, 2, null, \"str\", 2, \"xx\" ]\n"
        "] ]");
    }
    it "propagates -1 (error)"
    {
      i.string = "xx";
      t = fabr_altg("e", &i, _2x, _1x, _xerr, NULL);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"e\", -1, 0, 0, null, \"altg\", 0, [\n"
        "  [ \"2x\", 1, 0, 2, null, \"str\", 2, \"xx\" ],\n"
        "  [ \"1x\", 0, 0, 1, null, \"str\", 1, [] ],\n"
        "  [ null, -1, 0, 0, null, \"_xerr\", 0, [] ]\n"
        "] ]");
    }

    it "propagates -1 (error) immediately"
    {
      i.string = "xx";
      t = fabr_altg("e", &i, _xerr, _2x, _1x, NULL);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"e\", -1, 0, 0, null, \"altg\", 0, [\n"
        "  [ null, -1, 0, 0, null, \"_xerr\", 0, [] ]\n"
        "] ]");
    }
  }
}

