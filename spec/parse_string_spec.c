
//
// specifying aabro
//
// Tue Dec 17 22:56:18 JST 2013
//

#include "aabro.h"


describe "abr_p_string()"
{
  before each
  {
    abr_node *n = NULL;
  }
  after each
  {
    if (n != NULL) abr_node_free(n);
  }

  it "parses"
  {
    n = abr_p_string("xxx", 0, "xxx");

    ensure(n != NULL);
  }
}

