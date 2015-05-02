
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
    fabr_input i = { "toto", 0 };

    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  it "returns a tree with result == 0 in case of failure"
  {
    t = fabr_str(NULL, "tutu", &i);

    expect(t != NULL);
    expect(t->name == NULL);
    expect(t->result i== 0);
    expect(t->offset zu== 0);
    expect(t->length zu== 0);
  }

  it "returns a tree with result == 1 in case of success"
  {
    t = fabr_str(NULL, "toto", &i);

    expect(t != NULL);
    expect(t->name == NULL);
    expect(t->result i== 1);
    expect(t->offset zu== 0);
    expect(t->length zu== 4);
  }

  it "names the result if there is a name"
  {
    t = fabr_str("x", "toto", &i);

    expect(t != NULL);
    expect(t->name === "x");
    expect(t->result i== 1);
    expect(t->offset zu== 0);
    expect(t->length zu== 4);
  }

  it "doesn't name in case of failure"
  {
    t = fabr_str("x", "tutu", &i);

    expect(t != NULL);
    expect(t->name == NULL);
    expect(t->result i== 0);
    expect(t->offset zu== 0);
    expect(t->length zu== 0);
  }
}

