
//
// specifying aabro
//
// Thu Jun 18 06:49:59 JST 2015
// Joyeux Anniversaire Steve!
//

#include "aabro.h"


describe "fabr_rename()"
{
  static fabr_tree *stoto(fabr_input *i) { return fabr_str(NULL, i, "toto"); }
  static fabr_tree *sntoto(fabr_input *i) { return fabr_str("s", i, "toto"); }

  before each
  {
    fabr_input i = { "toto", 0, 0 };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  it "renames (over NULL)"
  {
    t = fabr_rename("x", &i, stoto);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"str\", 4, \"toto\" ]");
  }

  it "renames (over a string)"
  {
    t = fabr_rename("y", &i, sntoto);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"y\", 1, 0, 4, null, \"str\", 4, \"toto\" ]");
  }

  it "removes a name"
  {
    t = fabr_rename(NULL, &i, sntoto);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ null, 1, 0, 4, null, \"str\", 4, \"toto\" ]");
  }
}

