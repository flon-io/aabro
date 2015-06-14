
//
// specifying aabro
//
// Tue May 19 21:19:21 JST 2015
//

#include "aabro.h"


describe "fabr_jseq()"
{
  before each
  {
    fabr_input i = { "tato", 0, 0 };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  static fabr_tree *_pip(fabr_input *i) { return fabr_str(NULL, i, "|"); }
  static fabr_tree *_a(fabr_input *i) { return fabr_str("a", i, "a"); }
  static fabr_tree *_com(fabr_input *i) { return fabr_str(NULL, i, ","); }
  //
  static fabr_tree *_as(fabr_input *i)
  {
    return fabr_jseq("as", i, _a, _com);
  }
  static fabr_tree *_ar(fabr_input *i)
  {
    return fabr_seq("ar", i, _pip, _as, _pip, NULL);
  }

  it "fails"
  {
    i.string = "b";
    t = fabr_jseq("x", &i, _a, _com);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"eseq\", 0, [\n"
      "  [ \"a\", 0, 0, 0, null, \"str\", 1, [] ]\n"
      "] ]");
  }

  it "fails (2)"
  {
    i.string = "a,ba";
    t = fabr_jseq("x", &i, _a, _com);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"eseq\", 0, [\n"
      "  [ \"a\", 1, 0, 1, null, \"str\", 1, \"a\" ],\n"
      "  [ null, 1, 1, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"a\", 0, 2, 0, null, \"str\", 1, [] ]\n"
      "] ]");
  }

  it "succeeds"
  {
    i.string = "a,a,a";
    t = fabr_jseq("x", &i, _a, _com);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 5, null, \"eseq\", 0, [\n"
      "  [ \"a\", 1, 0, 1, null, \"str\", 1, \"a\" ],\n"
      "  [ null, 1, 1, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"a\", 1, 2, 1, null, \"str\", 1, \"a\" ],\n"
      "  [ null, 1, 3, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"a\", 1, 4, 1, null, \"str\", 1, \"a\" ],\n"
      "  [ null, 0, 5, 0, null, \"str\", 1, [] ]\n"
      "] ]");
  }

  it "succeeds (prune)"
  {
    i.string = "a,a,a";
    i.flags = FABR_F_PRUNE;

    t = fabr_jseq("x", &i, _a, _com);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 5, null, \"eseq\", 0, [\n"
      "  [ \"a\", 1, 0, 1, null, \"str\", 1, \"a\" ],\n"
      "  [ null, 1, 1, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"a\", 1, 2, 1, null, \"str\", 1, \"a\" ],\n"
      "  [ null, 1, 3, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"a\", 1, 4, 1, null, \"str\", 1, \"a\" ]\n"
      "] ]");
  }

  it "succeeds (bigger context)"
  {
    i.string = "|a,a,a|";
    t = _ar(&i);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"ar\", 1, 0, 7, null, \"seq\", 0, [\n"
      "  [ null, 1, 0, 1, null, \"str\", 1, \"|\" ],\n"
      "  [ \"as\", 1, 1, 5, null, \"eseq\", 0, [\n"
      "    [ \"a\", 1, 1, 1, null, \"str\", 1, \"a\" ],\n"
      "    [ null, 1, 2, 1, null, \"str\", 1, \",\" ],\n"
      "    [ \"a\", 1, 3, 1, null, \"str\", 1, \"a\" ],\n"
      "    [ null, 1, 4, 1, null, \"str\", 1, \",\" ],\n"
      "    [ \"a\", 1, 5, 1, null, \"str\", 1, \"a\" ],\n"
      "    [ null, 0, 6, 0, null, \"str\", 1, [] ]\n"
      "  ] ],\n"
      "  [ null, 1, 6, 1, null, \"str\", 1, \"|\" ]\n"
      "] ]");
  }

  it "resets the input offset in case of failure"
  {
    i.string = "a,ba";
    t = fabr_jseq("x", &i, _a, _com);

    expect(t->result i== 0);
    expect(t->length zu== 0);
    expect(i.offset zu== 0);
  }

  it "prunes"
  {
    i.string = "|a,a|";
    i.flags = FABR_F_PRUNE;

    t = _ar(&i);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"ar\", 1, 0, 5, null, \"seq\", 0, [\n"
      "  [ null, 1, 0, 1, null, \"str\", 1, \"|\" ],\n"
      "  [ \"as\", 1, 1, 3, null, \"eseq\", 0, [\n"
      "    [ \"a\", 1, 1, 1, null, \"str\", 1, \"a\" ],\n"
      "    [ null, 1, 2, 1, null, \"str\", 1, \",\" ],\n"
      "    [ \"a\", 1, 3, 1, null, \"str\", 1, \"a\" ]\n"
      "  ] ],\n"
      "  [ null, 1, 4, 1, null, \"str\", 1, \"|\" ]\n"
      "] ]");
  }
}

