
//
// specifying aabro
//
// Wed Dec 18 16:19:13 JST 2013
//

#include "aabro.h"


describe "abr_node"
{
  describe "abr_malloc_node()"
  {
    it "creates a node"
    {
      abr_node *n = abr_malloc_node("nada", 0);

      ensure(n != NULL);

      abr_free_node(n);
    }
  }
  describe "abr_node_to_string(n)"
  {
    it "returns a string representation of the node"
    {
      abr_node *n = abr_malloc_node("nada", 0);
      char *s = abr_node_to_string(n);

      ensure(strcmp("[ nada, 0 ]", s) == 0);

      free(s);
      abr_free_node(n);
    }
  }
}

