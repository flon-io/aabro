
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
    abr_tree *t = NULL;
    abr_parser *p = NULL;
    char *s = NULL;
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
    if (p != NULL) abr_parser_free(p);
    if (s != NULL) free(s);
  }

  it "parses"
  {
    p = abr_string("xxx");
    t = abr_parse("xxx", 0, p);
    s = abr_tree_to_string(t);

    ensure(s === "[ (null), 1, 0, 3 ]");
  }

  it "fails to parse"
  {
    //n = abr_p_string("yyy", 0, "xxx");
    //s = abr_node_to_string(n);
    p = abr_string("xxx");
    t = abr_parse("yyy", 0, p);
    s = abr_tree_to_string(t);

    ensure(s === "[ (null), 0, 0, -1 ]");
  }
}

