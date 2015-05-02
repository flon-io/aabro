
//
// specifying aabro
//
// Fri Apr 11 06:01:20 JST 2014
//

#include "aabro.h"


describe "fabr_seq()"
{
  before each
  {
    fabr_input i = { "tato", 0 };

    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  static fabr_tree *_ta(fabr_input *i) { return fabr_str(NULL, "ta", i); }
  static fabr_tree *_to(fabr_input *i) { return fabr_str(NULL, "to", i); }

  it "returns a tree with result == 0 in case of failure"
  {
    t = fabr_seq(NULL, &i, _to, _ta, NULL);

    expect(t != NULL);
    expect(t->name == NULL);
    expect(t->result i== 0);
    expect(t->offset zu== 0);
    expect(t->length zu== 0);
  }

  it "returns a tree with result == 1 in case of success"
  {
    t = fabr_seq(NULL, &i, _ta, _to, NULL);

    expect(t != NULL);
    expect(t->name == NULL);
    expect(t->result i== 1);
    expect(t->offset zu== 0);
    expect(t->length zu== 4);
  }

  it "names the result if there is a name"
  {
    t = fabr_seq("x", &i, _ta, _to, NULL);

    expect(t != NULL);
    expect(t->name === "x");
  }

  it "doesn't name in case of failure"
  {
    t = fabr_seq("x", &i, _to, _ta, NULL);

    expect(t != NULL);
    expect(t->name == NULL);
  }
}

