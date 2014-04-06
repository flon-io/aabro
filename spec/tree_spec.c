
//
// specifying aabro
//
// Wed Dec 18 16:19:13 JST 2013
//

#include "aabro.h"


describe "abr_tree"
{
  before each
  {
    abr_tree *t = NULL;
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
  }

  describe "abr_tree_malloc()"
  {
    it "creates a tree struct"
    {
      t = abr_tree_malloc(0, 1, 2, NULL);

      ensure(t != NULL);
    }
  }
  describe "abr_tree_to_string(t)"
  {
    it "returns a string representation of the tree struct"
    {
      t = abr_tree_malloc(0, 1, 2, NULL);

      ensure(abr_tree_to_string(t) ===f "[ (null), 0, 1, 2, [] ]");
    }
  }
}

