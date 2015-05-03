
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
    fabr_input i = { "", 0 };
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
      "[ \"x\", 0, 0, 0, null, \"rng\", [] ]");
  }

  it "accepts \"abc\" (success)"
  {
    i.string = "aa";
    t = fabr_rng("x", &i, "abc");

    char *s = fabr_tree_to_string(t, NULL, 0);

    ensure(s ===f ""
      "[ \"x\", 1, 0, 1, null, \"rng\", [] ]");
  }

  it "accepts \"a-z\""
  it "accepts \"a-z0-9X\""
  it "accepts \"\\-\\[\""
  it "accepts \"\\--\\[\""
  it "accepts \"^a-z\""
  it "accepts \"\\^a-z\""
  it "accepts \"+-\""
  it "accepts \".\" (a dot on its own, any char except newline)"
  it "accepts \"$\" (dollar on its own, the end of the input)"
  it "accepts \"\\\\\" (backslashing the backslash)"
}
//      it "is successful when the input starts with a char in the range"
//      {
//        p = fabr_range("abc");
//        t = fabr_parse("aaa", 0, p);
//
//        ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
//          "[ null, 1, 0, 1, null, \"range-0\", [] ]");
//      }
//
//      it "fails when the input does not start with a char in the range"
//      {
//        p = fabr_range("abc");
//        t = fabr_parse("daa", 0, p);
//
//        ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
//          "[ null, 0, 0, 0, null, \"range-0\", [] ]");
//      }
//
//      it "accepts \"a-z\""
//      {
//        p = fabr_range("a-z");
//
//        t = fabr_parse("aaa", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("zaa", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("Aaa", 0, p);
//
//        ensure(t->result == 0);
//      }
//
//      it "accepts \"a-z0-9X\""
//      {
//        p = fabr_range("a-z0-9X");
//
//        t = fabr_parse("a", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("9", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("X", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("-", 0, p);
//
//        ensure(t->result == 0);
//      }
//
//      it "accepts \"\\-\\[\""
//      {
//        p = fabr_range("\\-\\[");
//
//        t = fabr_parse("a", 0, p);
//
//        ensure(t->result == 0);
//
//        fabr_tree_free(t);
//        t = fabr_parse("-", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("[", 0, p);
//
//        ensure(t->result == 1);
//      }
//
//      it "accepts \"\\--\\[\""
//      {
//        p = fabr_range("\\--\\[");
//
//        t = fabr_parse("a", 0, p);
//
//        ensure(t->result == 0);
//
//        fabr_tree_free(t);
//        t = fabr_parse("-", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("[", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("A", 0, p);
//
//        ensure(t->result == 1);
//      }
//
//      it "accepts \"^a-z\""
//      {
//        p = fabr_range("^a-z");
//
//        t = fabr_parse("a", 0, p);
//
//        ensure(t->result == 0);
//
//        fabr_tree_free(t);
//        t = fabr_parse("A", 0, p);
//
//        ensure(t->result == 1);
//      }
//
//      it "accepts \"\\^a-z\""
//      {
//        p = fabr_range("\\^a-z");
//
//        t = fabr_parse("a", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("^", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("A", 0, p);
//
//        ensure(t->result == 0);
//      }
//
//      it "accepts \"+-\""
//      {
//        p = fabr_range("+-");
//
//        t = fabr_parse("+", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("-", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("*", 0, p);
//
//        ensure(t->result == 0);
//      }
//
//      it "accepts \".\" (a dot on its own, any char except newline)"
//      {
//        p = fabr_range(".");
//
//        t = fabr_parse("a", 0, p);
//
//        ensure(t->result == 1);
//
//        fabr_tree_free(t);
//        t = fabr_parse("\n", 0, p);
//
//        ensure(t->result == 0);
//
//        fabr_tree_free(t);
//        t = fabr_parse("\0", 0, p);
//
//        ensure(t->result == 0);
//      }
//
//      it "accepts \"$\" (dollar on its own, the end of the input)"
//      {
//        p = fabr_range("$");
//
//        t = fabr_parse("", 0, p);
//
//        ensure(t->result == 1);
//        ensure(t->length == 0);
//
//        fabr_tree_free(t);
//        t = fabr_parse("\0", 0, p);
//
//        ensure(t->result == 1);
//        ensure(t->length == 0);
//
//        fabr_tree_free(t);
//        t = fabr_parse("a", 0, p);
//
//        ensure(t->result == 0);
//        ensure(t->length == 0);
//      }
//
//      it "accepts \"\\\\\" (backslashing the backslash)"
//      {
//        p = fabr_range("\\\\");
//
//        t = fabr_parse("a", 0, p);
//
//        ensure(t->result == 0);
//
//        fabr_tree_free(t);
//        t = fabr_parse("\\", 0, p);
//
//        ensure(t->result == 1);
//      }

