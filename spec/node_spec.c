
//
// specifying aabro
//
// Wed Dec 18 16:19:13 JST 2013
//

#include "aabro.h"


describe "abr_node"
{
  before each
  {
    abr_node *n = NULL;
  }
  after each
  {
    if (n != NULL) abr_node_free(n);
  }

  describe "abr_node_malloc_node()"
  {
    it "creates a node"
    {
      n = abr_node_malloc(0, 1, 2);

      ensure(n != NULL);
    }
  }
  describe "abr_node_to_string(n)"
  {
    it "returns a string representation of the node"
    {
      n = abr_node_malloc(0, 1, 2);

      ensure(abr_node_to_string(n) ===f "[ (null), 0, 1, 2 ]");
    }
  }
}

