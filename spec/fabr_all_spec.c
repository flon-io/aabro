
//
// specifying aabro
//
// Wed May 20 06:09:13 JST 2015
//

#include "aabro.h"


describe "fabr_all()"
{
  before each
  {
    fabr_input i = { "", 0, };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  static fabr_tree *_b(fabr_input *i) { return fabr_str("b", i, "b"); }
  static fabr_tree *_minus(fabr_input *i) { return fabr_rex("-", i, "[a"); }

  it "returns a wrapped tree with result == 1 in case of success"
  {
    i.string = "b";

    t = fabr_all("x", &i, _b);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 1, null, \"all\", 0, [\n"
      "  [ \"b\", 1, 0, 1, null, \"str\", 1, \"b\" ]\n"
      "] ]");
  }

  it "returns a wrapped tree with result == 0 in case of remaining input"
  {
    i.string = "bb";

    t = fabr_all("x", &i, _b);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"all\", 0, [\n"
      "  [ \"b\", 1, 0, 1, null, \"str\", 1, \"b\" ]\n"
      "] ]");
  }

  it "returns a wrapped tree with result == 0 in case of upstream failure"
  {
    i.string = "c";

    t = fabr_all("x", &i, _b);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"all\", 0, [\n"
      "  [ \"b\", 0, 0, 0, null, \"str\", 1, [] ]\n"
      "] ]");
  }

  it "returns a wrapped tree with result == -1 in case of upstream error"
  {
    i.string = "whatever";

    t = fabr_all("x", &i, _minus);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", -1, 0, 0, null, \"all\", 0, [\n"
      "  [ \"-\", -1, 0, 0, null, \"rex\", 2, [\n"
      "    [ null, -1, 0, 0, null, \"rex_seq\", 2, [\n"
      "      [ null, -1, 0, 0, \"range not closed >[a<2\", \"rex_rep\", 0, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }
}

