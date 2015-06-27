
//
// specifying aabro
//
// Sat Jun 27 17:27:26 JST 2015
//

#include "aabro.h"


describe "fabr_prune()"
{
  before each
  {
    fabr_input i = { NULL, 0, 0 };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  static fabr_tree *_y1(fabr_input *i) { return fabr_str("y1", i, "y"); }
  static fabr_tree *_y2(fabr_input *i) { return fabr_str("y2", i, "yy"); }
  static fabr_tree *_y3(fabr_input *i) { return fabr_str("y3", i, "yyy"); }

  it "drops the short winners when pruning (2)"
  {
    i.string = "yy";
    t = fabr_altg(NULL, &i, 1, _y1, _y2, _y3, NULL);

    expect(i.offset zu== 2);

    fabr_puts(t, i.string, 3);
    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ null, 1, 0, 2, null, \"altg\", 0, [\n"
      "  [ \"y1\", 0, 0, 0, null, \"str\", 1, [] ],\n"
      "  [ \"y2\", 1, 0, 2, null, \"str\", 2, \"yy\" ],\n"
      "  [ \"y3\", 0, 0, 0, null, \"str\", 3, [] ]\n"
      "] ]");

    fabr_prune(t);

    fabr_puts(t, i.string, 3);
    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ null, 1, 0, 2, null, \"altg\", 0, [\n"
      "  [ \"y2\", 1, 0, 2, null, \"str\", 2, \"yy\" ]\n"
      "] ]");
  }
}

