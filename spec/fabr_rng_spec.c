
//
// specifying aabro
//
// Fri Aug  8 16:40:47 JST 2014
//

#include "aabro.h"


describe "fabr_rng()"
{
  before each
  {
    fabr_input i = { "", 0, 0 };
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  it "accepts \"abc\" (failure)"
  {
    i.string = "d";
    t = fabr_rng("x", &i, "abc");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 3, [] ]");
  }

  it "accepts \"abc\" (success)"
  {
    i.string = "aa";
    t = fabr_rng("x", &i, "abc");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 3, [] ]");
  }

  it "accepts \"a-z\" (failure)"
  {
    i.string = "1";
    t = fabr_rng("x", &i, "a-z");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 3, [] ]");
  }

  it "accepts \"a-z\" (success)"
  {
    i.string = "a";
    t = fabr_rng("x", &i, "a-z");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 3, [] ]");
  }

  it "accepts \"a-z0-9X\" (failure)"
  {
    i.string = "Y";
    t = fabr_rng("x", &i, "a-z0-9X");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 7, [] ]");
  }

  it "accepts \"a-z0-9X\" (success)"
  {
    i.string = "8";
    t = fabr_rng("x", &i, "a-z0-9X");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 7, [] ]");
  }

  it "accepts \"\\-\\[\" (failure)"
  {
    i.string = "a";
    t = fabr_rng("x", &i, "\\-\\[");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 4, [] ]");
  }

  it "accepts \"\\-\\[\" (success)"
  {
    i.string = "-";
    t = fabr_rng("x", &i, "\\-\\[");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 4, [] ]");
  }

  it "accepts \"\\-\\[\" (success 2)"
  {
    i.string = "[";
    t = fabr_rng("x", &i, "\\-\\[");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 4, [] ]");
  }

  it "accepts \"\\--\\[\" (failure)"
  {
    i.string = "a";
    t = fabr_rng("x", &i, "\\--\\[");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 5, [] ]");
  }

  it "accepts \"\\--\\[\" (success)"
  {
    i.string = ".";
    t = fabr_rng("x", &i, "\\--\\[");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 5, [] ]");
  }

  it "accepts \"^a-z\" (failure)"
  {
    i.string = "z";
    t = fabr_rng("x", &i, "^a-z");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 4, [] ]");
  }

  it "accepts \"^a-z\" (success)"
  {
    i.string = "Z";
    t = fabr_rng("x", &i, "^a-z");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 4, [] ]");
  }

  it "accepts \"\\^a-z\" (failure)"
  {
    i.string = "Z";
    t = fabr_rng("x", &i, "\\^a-z");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 5, [] ]");
  }

  it "accepts \"\\^a-z\" (success)"
  {
    i.string = "^";
    t = fabr_rng("x", &i, "\\^a-z");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 5, [] ]");
  }

  it "accepts \"+-\" (failure)"
  {
    i.string = "*";
    t = fabr_rng("x", &i, "+-");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 2, [] ]");
  }

  it "accepts \"+-\" (success)"
  {
    i.string = "-";
    t = fabr_rng("x", &i, "+-");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 2, [] ]");
  }

  it "accepts \".\" (a dot on its own, any char except newline) (failure)"
  {
    i.string = "\n";
    t = fabr_rng("x", &i, ".");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 1, [] ]");
  }

  it "accepts \".\" (a dot on its own, any char except newline) (success)"
  {
    i.string = "*";
    t = fabr_rng("x", &i, ".");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 1, [] ]");
  }

  it "accepts \"$\" (dollar on its own, the end of the input) (failure)"
  {
    i.string = "A";
    t = fabr_rng("x", &i, "$");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 1, [] ]");
  }

  it "accepts \"$\" (dollar on its own, the end of the input) (success)"
  {
    i.string = "";
    t = fabr_rng("x", &i, "$");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 0, null, \"rng\", 1, [] ]");
  }

  it "accepts \"\\\\\" (backslashing the backslash) (failure)"
  {
    i.string = "A";
    t = fabr_rng("x", &i, "\\\\");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 0, 0, 0, null, \"rng\", 2, [] ]");
  }

  it "accepts \"\\\\\" (backslashing the backslash) (success)"
  {
    i.string = "\\";
    t = fabr_rng("x", &i, "\\\\");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", 2, [] ]");
  }
}

