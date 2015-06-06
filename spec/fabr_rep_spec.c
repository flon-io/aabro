
//
// specifying aabro
//
// Wed Apr  2 06:25:31 JST 2014
//

#include "aabro.h"


describe "fabr_rep()"
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

  static fabr_tree *_t0(fabr_input *i)
  {
    return fabr_str(NULL, i, "t0");
  }

  static fabr_tree *_terr(fabr_input *i)
  {
    //fabr_tree *r = fabr_tree_malloc(NULL, "_terr", i, 0);
    fabr_tree *r = calloc(1, sizeof(fabr_tree));
    r->result = -1;
    r->parter = "_terr";
    r->offset = i->offset;

    return r;
  }

  it "returns a tree with result == 0 in case of failure"
  {
    i.string = "t1t1t1";

    t = fabr_rep("x", &i, _t0, 1, 2);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rep\", 0, [\n"
      "  [ null, 0, 0, 0, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "returns a tree with result == 1 in case of success"
  {
    i.string = "t0t0t0";

    t = fabr_rep("x", &i, _t0, 1, 2);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 1, 0, 4, null, \"rep\", 0, [\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 2, 2, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "fails midway"
  {
    i.string = "t0t1t0";

    t = fabr_rep("x", &i, _t0, 2, 3);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rep\", 0, [\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 0, 2, 0, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "fails when not enough"
  {
    i.string = "t0t0t0";

    t = fabr_rep("x", &i, _t0, 4, 0);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rep\", 0, [\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 2, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 4, 2, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "succeeds when max is open"
  {
    i.string = "t0t0t0";

    t = fabr_rep("x", &i, _t0, 2, 0);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 1, 0, 6, null, \"rep\", 0, [\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 2, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 4, 2, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "succeeds when {0,1}"
  {
    i.string = "t1";

    t = fabr_rep("x", &i, _t0, 0, 1);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 1, 0, 0, null, \"rep\", 0, [\n"
      "  [ null, 0, 0, 0, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "prunes the last unsuccessful try (when the prune flag is on)"
  {
    i.string = "t0t0t0t1t1";
    i.flags = FABR_F_PRUNE;

    t = fabr_rep("x", &i, _t0, 2, 0);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 1, 0, 6, null, \"rep\", 0, [\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 2, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 4, 2, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "propagates errors"
  {
    i.string = "t2";

    t = fabr_rep("x", &i, _terr, 0, 1);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", -1, 0, 0, null, \"rep\", 0, [\n"
      "  [ null, -1, 0, 0, null, \"_terr\", 0, [] ]\n"
      "] ]");
  }

  it "resets the input offset in case of failure"
  {
    i.string = "t0t0t0";

    t = fabr_rep("x", &i, _t0, 4, 0);

    expect(t->result i== 0);
    expect(t->length zu== 0);
    expect(i.offset zu== 0);
  }

  it "accepts an empty input"
  {
    i.string = "t0t0";
    i.offset = 4;

    t = fabr_rep("z", &i, _t0, 4, 0);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 0, 4, 0, null, \"rep\", 0, [] ]");
  }
}

