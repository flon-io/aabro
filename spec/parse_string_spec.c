
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
    char *s = NULL;
  }
  after each
  {
    if (n != NULL) abr_node_free(n);
    if (s != NULL) free(s);
  }

  it "parses"
  {
    n = abr_p_string("xxx", 0, "xxx");
    s = abr_node_to_string(n);

    ensure(s === "[ (null), 1, 0, 3 ]");
  }

  it "fails to parse"
  {
    n = abr_p_string("yyy", 0, "xxx");
    s = abr_node_to_string(n);

    ensure(s === "[ (null), 0, 0, -1 ]");
  }
}

