
//
// specifying aabro
//
// Thu Apr 10 05:47:54 JST 2014
//

#include "aabro.h"


describe "fabr_alt()"
{
  before each
  {
    fabr_input i = { "tz", 0 };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  fabr_tree *_tx(fabr_input *i) { return fabr_str(NULL, i, "tx"); }
  fabr_tree *_ty(fabr_input *i) { return fabr_str(NULL, i, "ty"); }
  fabr_tree *_tz(fabr_input *i) { return fabr_str(NULL, i, "tz"); }

  it "returns a tree with result == 0 in case of failure"
  {
    t = fabr_alt("x", &i, _tx, _ty, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"alt\", 0, [\n"
      "  [ null, 0, 0, 0, null, \"str\", 2, [] ],\n"
      "  [ null, 0, 0, 0, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "returns a tree with result == 1 in case of success"
  {
    t = fabr_alt("x", &i, _ty, _tz, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 2, null, \"alt\", 0, [\n"
      "  [ null, 0, 0, 0, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ]\n"
      "] ]");
  }
}

