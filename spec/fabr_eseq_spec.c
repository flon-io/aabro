
//
// specifying aabro
//
// Thu Jun  4 07:01:32 JST 2015
//

#include "aabro.h"


describe "fabr_eseq()"
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

  static fabr_tree *_es_sep(fabr_input *i) { return fabr_str(NULL, i, ","); }
  static fabr_tree *_es_sta(fabr_input *i) { return fabr_str(NULL, i, "<"); }
  static fabr_tree *_es_end(fabr_input *i) { return fabr_str(NULL, i, ">"); }
  static fabr_tree *_es_elt(fabr_input *i) { return fabr_rng("e", i, "a-z"); }

  it "fails"
  {
    i.string = "b,c";
    t = fabr_eseq("z", &i, _es_sta, _es_elt, _es_sep, _es_end);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 0, 0, 0, null, \"eseq\", 0, [\n"
      "  [ null, 0, 0, 0, null, \"str\", 1, [] ]\n"
      "] ]");
  }

  it "fails (2)"
  {
    i.string = "<b,>";
    t = fabr_eseq("z", &i, _es_sta, _es_elt, _es_sep, _es_end);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 0, 0, 0, null, \"eseq\", 0, [\n"
      "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
      "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"b\" ],\n"
      "  [ null, 1, 2, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"e\", 0, 3, 0, null, \"rng\", 3, [] ]\n"
      "] ]");
  }

  it "succeeds"
  {
    i.string = "<a,b,c>";
    t = fabr_eseq("z", &i, _es_sta, _es_elt, _es_sep, _es_end);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 1, 0, 7, null, \"eseq\", 0, [\n"
      "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
      "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"a\" ],\n"
      "  [ null, 1, 2, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"e\", 1, 3, 1, null, \"rng\", 3, \"b\" ],\n"
      "  [ null, 1, 4, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"e\", 1, 5, 1, null, \"rng\", 3, \"c\" ],\n"
      "  [ null, 0, 6, 0, null, \"str\", 1, [] ],\n"
      "  [ null, 1, 6, 1, null, \"str\", 1, \">\" ]\n"
      "] ]");
  }

  it "succeeds (one element)"
  {
    i.string = "<a>";
    t = fabr_eseq("one", &i, _es_sta, _es_elt, _es_sep, _es_end);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"one\", 1, 0, 3, null, \"eseq\", 0, [\n"
      "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
      "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"a\" ],\n"
      "  [ null, 0, 2, 0, null, \"str\", 1, [] ],\n"
      "  [ null, 1, 2, 1, null, \"str\", 1, \">\" ]\n"
      "] ]");
  }

  it "succeeds (empty construct)"
  {
    i.string = "<>";
    t = fabr_eseq("z", &i, _es_sta, _es_elt, _es_sep, _es_end);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 1, 0, 2, null, \"eseq\", 0, [\n"
      "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
      "  [ \"e\", 0, 1, 0, null, \"rng\", 3, [] ],\n"
      "  [ null, 1, 1, 1, null, \"str\", 1, \">\" ]\n"
      "] ]");
  }

  it "succeeds (construct without start)"
  {
    i.string = "a,b>";
    t = fabr_eseq("z", &i, NULL, _es_elt, _es_sep, _es_end);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 1, 0, 4, null, \"eseq\", 0, [\n"
      "  [ \"e\", 1, 0, 1, null, \"rng\", 3, \"a\" ],\n"
      "  [ null, 1, 1, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"e\", 1, 2, 1, null, \"rng\", 3, \"b\" ],\n"
      "  [ null, 0, 3, 0, null, \"str\", 1, [] ],\n"
      "  [ null, 1, 3, 1, null, \"str\", 1, \">\" ]\n"
      "] ]");
  }

  it "succeeds (empty construct without start) (prunes)"
  {
    i.string = ">";
    i.flags = FABR_F_PRUNE;
    t = fabr_eseq("z", &i, NULL, _es_elt, _es_sep, _es_end);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 1, 0, 1, null, \"eseq\", 0, [\n"
      //"  [ \"e\", 0, 0, 0, null, \"rng\", 3, [] ],\n"
      "  [ null, 1, 0, 1, null, \"str\", 1, \">\" ]\n"
      "] ]");
  }

  it "resets the input offset in case of failure"
  {
    i.string = "<b,>";
    t = fabr_eseq("z", &i, _es_sta, _es_elt, _es_sep, _es_end);

    expect(t->result i== 0);
    expect(t->length zu== 0);
    expect(i.offset zu== 0);
  }

  it "prunes"
  {
    i.string = "<a,b>";
    i.flags = FABR_F_PRUNE;

    t = fabr_eseq("z", &i, _es_sta, _es_elt, _es_sep, _es_end);

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 1, 0, 5, null, \"eseq\", 0, [\n"
      "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
      "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"a\" ],\n"
      "  [ null, 1, 2, 1, null, \"str\", 1, \",\" ],\n"
      "  [ \"e\", 1, 3, 1, null, \"rng\", 3, \"b\" ],\n"
      "  [ null, 1, 4, 1, null, \"str\", 1, \">\" ]\n"
      "] ]");
  }

  it "accepts an empty input"
  {
    i.string = "ab";
    i.offset = 2;

    t = fabr_eseq("z", &i, _es_sta, _es_elt, _es_sep, _es_end);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 0, 2, 0, null, \"eseq\", 0, [\n"
      "  [ null, 0, 2, 0, null, \"str\", 1, [] ]\n"
      "] ]");
  }

  //
  // zero-length spec'ing

  static fabr_tree *_es_se(fabr_input *i) { return fabr_rex(NULL, i, ",?"); }
  static fabr_tree *_es_el(fabr_input *i) { return fabr_rex("e", i, "a?"); }

  context "with zero-length separators"
  {
    it "checks for the end anyway (failure)"
    {
      i.string = "<a,b...";

      t = fabr_eseq("Z", &i, _es_sta, _es_elt, _es_se, _es_end);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"Z\", 0, 0, 0, null, \"eseq\", 0, [\n"
        "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
        "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"a\" ],\n"
        "  [ null, 1, 2, 1, null, \"rex\", 2, [\n"
        "    [ null, 1, 2, 1, null, \"rex_seq\", 2, [\n"
        "      [ null, 1, 2, 1, null, \"rex_rep\", 2, [\n"
        "        [ null, 1, 2, 1, null, \"rex_str\", 1, \",\" ]\n"
        "      ] ]\n"
        "    ] ]\n"
        "  ] ],\n"
        "  [ \"e\", 1, 3, 1, null, \"rng\", 3, \"b\" ],\n"
        "  [ null, 0, 4, 0, null, \"str\", 1, [] ]\n"
        "] ]");
    }

    it "checks for the end anyway (success)"
    {
      i.string = "<a,b>";

      t = fabr_eseq("Z", &i, _es_sta, _es_elt, _es_se, _es_end);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"Z\", 1, 0, 5, null, \"eseq\", 0, [\n"
        "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
        "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"a\" ],\n"
        "  [ null, 1, 2, 1, null, \"rex\", 2, [\n"
        "    [ null, 1, 2, 1, null, \"rex_seq\", 2, [\n"
        "      [ null, 1, 2, 1, null, \"rex_rep\", 2, [\n"
        "        [ null, 1, 2, 1, null, \"rex_str\", 1, \",\" ]\n"
        "      ] ]\n"
        "    ] ]\n"
        "  ] ],\n"
        "  [ \"e\", 1, 3, 1, null, \"rng\", 3, \"b\" ],\n"
        "  [ null, 1, 4, 1, null, \"str\", 1, \">\" ]\n"
        "] ]");
    }

    it "checks for the end anyway (succeeds and prunes)"
    {
      i.string = "<a,b>";
      i.flags = FABR_F_PRUNE;

      t = fabr_eseq("Z", &i, _es_sta, _es_elt, _es_se, _es_end);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"Z\", 1, 0, 5, null, \"eseq\", 0, [\n"
        "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
        "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"a\" ],\n"
        "  [ null, 1, 2, 1, null, \"rex\", 2, \",\" ],\n"
        "  [ \"e\", 1, 3, 1, null, \"rng\", 3, \"b\" ],\n"
        "  [ null, 1, 4, 1, null, \"str\", 1, \">\" ]\n"
        "] ]");
    }

    it "goes beyond the first element"
    {
      i.string = "<ab>";
      i.flags = FABR_F_PRUNE;

      t = fabr_eseq("Z", &i, _es_sta, _es_elt, _es_se, _es_end);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"Z\", 1, 0, 4, null, \"eseq\", 0, [\n"
        "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
        "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"a\" ],\n"
        "  [ null, 1, 2, 0, null, \"rex\", 2, \"\" ],\n"
        "  [ \"e\", 1, 2, 1, null, \"rng\", 3, \"b\" ],\n"
        "  [ null, 1, 3, 1, null, \"str\", 1, \">\" ]\n"
        "] ]");
    }

    it "goes beyond the first element (jseq)"
    {
      i.string = "ab";
      i.flags = FABR_F_PRUNE;

      t = fabr_jseq("Z", &i, _es_elt, _es_se);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"Z\", 1, 0, 2, null, \"eseq\", 0, [\n"
        "  [ \"e\", 1, 0, 1, null, \"rng\", 3, \"a\" ],\n"
        "  [ null, 1, 1, 0, null, \"rex\", 2, \"\" ],\n"
        "  [ \"e\", 1, 1, 1, null, \"rng\", 3, \"b\" ]\n"
        "] ]");
    }
  }

  context "with zero-length elements"
  {
    it "accepts 'blanks' between separators"
    {
      i.string = "<a,,a>";
      i.flags = FABR_F_PRUNE;

      t = fabr_eseq("Z", &i, _es_sta, _es_el, _es_sep, _es_end);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"Z\", 1, 0, 6, null, \"eseq\", 0, [\n"
        "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
        "  [ \"e\", 1, 1, 1, null, \"rex\", 2, \"a\" ],\n"
        "  [ null, 1, 2, 1, null, \"str\", 1, \",\" ],\n"
        "  [ \"e\", 1, 3, 0, null, \"rex\", 2, \"\" ],\n"
        "  [ null, 1, 3, 1, null, \"str\", 1, \",\" ],\n"
        "  [ \"e\", 1, 4, 1, null, \"rex\", 2, \"a\" ],\n"
        "  [ null, 1, 5, 1, null, \"str\", 1, \">\" ]\n"
        "] ]");
    }
  }

  context "with zero-length elements and separators"
  {
    it "works"
    {
      i.string = "<a,,a>";
      i.flags = FABR_F_PRUNE;

      t = fabr_eseq("B", &i, _es_sta, _es_el, _es_se, _es_end);

      ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"B\", 1, 0, 6, null, \"eseq\", 0, [\n"
        "  [ null, 1, 0, 1, null, \"str\", 1, \"<\" ],\n"
        "  [ \"e\", 1, 1, 1, null, \"rex\", 2, \"a\" ],\n"
        "  [ null, 1, 2, 1, null, \"rex\", 2, \",\" ],\n"
        "  [ \"e\", 1, 3, 0, null, \"rex\", 2, \"\" ],\n"
        "  [ null, 1, 3, 1, null, \"rex\", 2, \",\" ],\n"
        "  [ \"e\", 1, 4, 1, null, \"rex\", 2, \"a\" ],\n"
        "  [ null, 1, 5, 1, null, \"str\", 1, \">\" ]\n"
        "] ]");
    }
  }
}

