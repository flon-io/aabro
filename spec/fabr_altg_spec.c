
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

  static fabr_tree *_1x(fabr_input *i) { return fabr_str(NULL, i, "x"); }
  static fabr_tree *_2x(fabr_input *i) { return fabr_str(NULL, i, "xx"); }

  context "greedy=0"
  {
    it "takes the first succesful result"
    {
      i.string = "xx";
      t = fabr_altg("g", &i, 0, _1x, _2x, NULL);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 1, null, \"alt\", 0, [\n"
        "  [ null, 1, 0, 1, null, \"str\", 1, \"x\" ]\n"
        "] ]");
    }
  }

  context "greedy=1"
  {
    it "favours the longest successful result"
    {
      i.string = "xx";
      t = fabr_altg("g", &i, 1, _1x, _2x, NULL);

      //fabr_puts(t, i.string, 3);
      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 2, null, \"altg\", 0, [\n"
        "  [ null, 0, 0, 1, null, \"str\", 1, [] ],\n"
        "  [ null, 1, 0, 2, null, \"str\", 2, \"xx\" ]\n"
        "] ]");
    }

    it "drops the short winners when pruning"
    {
      i.string = "xx";
      i.flags = FABR_F_PRUNE;
      t = fabr_altg("g", &i, 1, _1x, _2x, NULL);

      //fabr_puts(t, i.string, 3);
      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 2, null, \"altg\", 0, [\n"
        "  [ null, 1, 0, 2, null, \"str\", 2, \"xx\" ]\n"
        "] ]");
    }

    it "drops the short winners when pruning" // prevent infinite pruning loop
    {
      i.string = "xx";
      i.flags = FABR_F_PRUNE;
      t = fabr_altg("g", &i, 1, _2x, _2x, NULL);

      //fabr_puts(t, i.string, 3);
      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"g\", 1, 0, 2, null, \"altg\", 0, [\n"
        "  [ null, 1, 0, 2, null, \"str\", 2, \"xx\" ]\n"
        "] ]");
    }
  }
}

