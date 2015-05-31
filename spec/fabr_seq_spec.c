
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
    fabr_input i = { "tato", 0, };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  fabr_tree *_ta(fabr_input *i) { return fabr_str(NULL, i, "ta"); }
  fabr_tree *_to(fabr_input *i) { return fabr_str(NULL, i, "to"); }

  it "returns a tree with result == 0 in case of failure"
  {
    t = fabr_seq(NULL, &i, _to, _ta, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ null, 0, 0, 0, null, \"seq\", 0, [\n"
      "  [ null, 0, 0, 0, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "returns a tree with result == 0 in case of failure (at 2nd step)"
  {
    t = fabr_seq(NULL, &i, _ta, _ta, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ null, 0, 0, 0, null, \"seq\", 0, [\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 0, 2, 0, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "returns a tree with result == 1 in case of success"
  {
    t = fabr_seq(NULL, &i, _ta, _to, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ null, 1, 0, 4, null, \"seq\", 0, [\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 2, 2, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "names the result if there is a name"
  {
    t = fabr_seq("x", &i, _ta, _to, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 1, 0, 4, null, \"seq\", 0, [\n"
      "  [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "  [ null, 1, 2, 2, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "names in case of failure as well"
  {
    t = fabr_seq("x", &i, _to, _ta, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    expect(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"seq\", 0, [\n"
      "  [ null, 0, 0, 0, null, \"str\", 2, [] ]\n"
      "] ]");
  }

  it "resets the input offset in case of failure"
  {
    i.string = "totita";
    t = fabr_seq("x", &i, _to, _ta, NULL);

    expect(t->result i== 0);
    expect(t->length zu== 0);
    expect(i.offset zu== 0);
  }

  context "quantifiers"
  {
    describe "fabr_qmark"
    {
      it "succeeds"

      it "succeeds (2)"
//      {
//        i.string = "tatota";
//        t = fabr_seq("y", &i, _ta, _to, fabr_qmark, _ta, NULL);
//
//
//        expect(fabr_tree_to_string(t, i.string, 0) ===f ""
//          "[ \"x\", 1, 0, 6, null, \"seq\", 0, [\n"
//          "  [ null, 1, 0, 2, null, \"str\", 2, \"ta\" ],\n"
//          "  [ null, 1, 2, 2, null, \"str\", 2, \"to\" ],\n"
//          "  [ null, 1, 4, 2, null, \"str\", 2, \"ta\" ]\n"
//          "] ]");
//      }
    }

    describe "fabr_star"
    {
    }

    describe "fabr_plus"
    {
    }
  }
}

