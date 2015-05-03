
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

  static fabr_tree *_ta(fabr_input *i) { return fabr_str(NULL, i, "ta"); }
  static fabr_tree *_to(fabr_input *i) { return fabr_str(NULL, i, "to"); }

  it "returns a tree with result == 0 in case of failure"
  {
    t = fabr_seq(NULL, &i, _to, _ta, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ null, 0, 0, 0, null, \"seq\", [\n"
      "  [ null, 0, 0, 0, null, \"str\", [] ]\n"
      "] ]");
  }

  it "returns a tree with result == 0 in case of failure (at 2nd step)"
  {
    t = fabr_seq(NULL, &i, _ta, _ta, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ null, 0, 0, 0, null, \"seq\", [\n"
      "  [ null, 1, 0, 2, null, \"str\", [] ],\n"
      "  [ null, 0, 2, 0, null, \"str\", [] ]\n"
      "] ]");
  }

  it "returns a tree with result == 1 in case of success"
  {
    t = fabr_seq(NULL, &i, _ta, _to, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ null, 1, 0, 4, null, \"seq\", [\n"
      "  [ null, 1, 0, 2, null, \"str\", [] ],\n"
      "  [ null, 1, 2, 2, null, \"str\", [] ]\n"
      "] ]");
  }

  it "names the result if there is a name"
  {
    t = fabr_seq("x", &i, _ta, _to, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 4, null, \"seq\", [\n"
      "  [ null, 1, 0, 2, null, \"str\", [] ],\n"
      "  [ null, 1, 2, 2, null, \"str\", [] ]\n"
      "] ]");
  }

  it "names in case of failure as well"
  {
    t = fabr_seq("x", &i, _to, _ta, NULL);

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"seq\", [\n"
      "  [ null, 0, 0, 0, null, \"str\", [] ]\n"
      "] ]");
  }
}

