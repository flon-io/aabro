
//
// specifying aabro
//
// Sun Jun 28 10:48:59 JST 2015
//

#include "aabro.h"


describe "fabr_eos()"
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

  it "fails"
  {
    i.string = "stuff";

    t = fabr_eos("Z", &i);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"Z\", 0, 0, 0, null, \"eos\", 0, [] ]");
  }

  it "succeeds"
  {
    i.string = "";

    t = fabr_eos("Z", &i);

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"Z\", 1, 0, 0, null, \"eos\", 0, \"\" ]");
  }
}

