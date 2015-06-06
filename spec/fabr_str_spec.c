
//
// specifying aabro
//
// Tue Dec 17 22:56:18 JST 2013
//

#include "aabro.h"


describe "fabr_str()"
{
  before each
  {
    fabr_input i = { "toto", 0, 0 };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  it "returns a tree with result == 0 in case of failure"
  {
    t = fabr_str(NULL, &i, "tutu");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ null, 0, 0, 0, null, \"str\", 4, [] ]");
  }

  it "returns a tree with result == 1 in case of success"
  {
    t = fabr_str(NULL, &i, "toto");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ null, 1, 0, 4, null, \"str\", 4, [] ]");
  }

  it "names the result if there is a name"
  {
    t = fabr_str("x", &i, "toto");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 4, null, \"str\", 4, [] ]");
  }

  it "names in case of failure as well"
  {
    t = fabr_str("x", &i, "tutu");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"str\", 4, [] ]");
  }

  it "accepts an empty input"
  {
    i.string = "t0t0";
    i.offset = 4;

    t = fabr_str("z", &i, "nada");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"z\", 0, 4, 0, null, \"str\", 4, [] ]");
  }
}

