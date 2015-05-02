
//
// specifying aabro
//
// Fri Apr 11 06:01:20 JST 2014
//

#include "aabro.h"


describe "fabr_seq()"
{
  before each
  {
    fabr_input i = { "tato", 0 };
    fabr_tree *t = NULL;
    char *s = NULL;
  }
  after each
  {
    fabr_tree_free(t);
    free(s);
  }

  static fabr_tree *_ta(fabr_input *i) { return fabr_str(NULL, i, "ta"); }
  static fabr_tree *_to(fabr_input *i) { return fabr_str(NULL, i, "to"); }

  it "returns a tree with result == 0 in case of failure"
  {
    t = fabr_seq(NULL, &i, _to, _ta, NULL);

    expect(t != NULL);
    expect(t->name == NULL);
    expect(t->result i== 0);
    expect(t->offset zu== 0);
    expect(t->length zu== 0);

    s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ null, 1, 0, 2, null, \"seq-0\", [\n"
      "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
      "  [ null, 1, 1, 1, null, \"string-01\", [] ]\n"
      "] ]");
  }

  it "returns a tree with result == 1 in case of success"
  {
    t = fabr_seq(NULL, &i, _ta, _to, NULL);

    expect(t != NULL);
    expect(t->name == NULL);
    expect(t->result i== 1);
    expect(t->offset zu== 0);
    expect(t->length zu== 4);
  }

  it "names the result if there is a name"
  {
    t = fabr_seq("x", &i, _ta, _to, NULL);

    expect(t != NULL);
    expect(t->name === "x");
  }

  it "names in case of failure as well"
  {
    t = fabr_seq("x", &i, _to, _ta, NULL);

    expect(t != NULL);
    expect(t->name === "x");
  }

//  context "parsing"
//  {
//    it "succeeds"
//    {
//      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
//      t = fabr_parse("xy", 0, p);
//      char *s = fabr_tree_to_string(t, NULL, 0);
//
//      ensure(s ===f ""
//        "[ null, 1, 0, 2, null, \"seq-0\", [\n"
//        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
//        "  [ null, 1, 1, 1, null, \"string-01\", [] ]\n"
//        "] ]");
//    }
//
//    it "succeeds (named parser)"
//    {
//      p = fabr_n_seq("xtheny", fabr_string("x"), fabr_string("y"), NULL);
//      t = fabr_parse("xy", 0, p);
//      char *s = fabr_tree_to_string(t, NULL, 0);
//
//      ensure(s ===f ""
//        "[ \"xtheny\", 1, 0, 2, null, \"seq-0\", [\n"
//        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
//        "  [ null, 1, 1, 1, null, \"string-01\", [] ]\n"
//        "] ]");
//    }
//
//    it "fails"
//    {
//      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
//      t = fabr_parse("z", 0, p);
//      char *s = fabr_tree_to_string(t, NULL, 0);
//
//      ensure(s ===f ""
//        "[ null, 0, 0, 0, null, \"seq-0\", [] ]");
//    }
//
//    it "fails (2nd step)"
//    {
//      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
//      t = fabr_parse("xz", 0, p);
//      char *s = fabr_tree_to_string(t, NULL, 0);
//
//      ensure(s ===f ""
//        "[ null, 0, 0, 0, null, \"seq-0\", [] ]");
//    }
//
//    it "reports the failed tries if not FABR_F_PRUNE"
//    {
//      p = fabr_seq(fabr_string("x"), fabr_string("y"), NULL);
//      t = fabr_parse_f("xz", 0, p, FABR_F_ALL);
//      char *s = fabr_tree_to_string(t, NULL, 0);
//
//      ensure(s ===f ""
//        "[ null, 0, 0, 0, null, \"seq-0\", [\n"
//        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
//        "  [ null, 0, 1, 0, null, \"string-01\", [] ]\n"
//        "] ]");
//    }
//
//    it "propagates errors"
//    {
//      p = fabr_seq(fabr_string("x"), fabr_n("y"), NULL);
//      t = fabr_parse("xz", 0, p);
//      char *s = fabr_tree_to_string(t, NULL, 0);
//
//      ensure(s ===f ""
//        "[ null, -1, 0, 0, null, \"seq-0\", [\n"
//        "  [ null, 1, 0, 1, null, \"string-00\", [] ],\n"
//        "  [ \"y\", -1, 1, 0, \"unlinked fabr_n(\"y\")\", \"n-01\", [] ]\n"
//        "] ]");
//    }
//  }
}

