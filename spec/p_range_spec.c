
//
// specifying aabro
//
// Fri Aug  8 16:40:47 JST 2014
//

#include "aabro.h"


context "abr_range"
{
  before each
  {
    abr_tree *t = NULL;
    abr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
    if (p != NULL) abr_parser_free(p);
  }

  describe "abr_range(s)"
  {
    it "creates a range parser"
    {
      p = abr_range("abc");

      ensure(p->type == 11);
      ensure(p->string === "abc");
      ensure(abr_parser_to_string(p) ===f "abr_range(\"abc\")");
    }

    context "parsing"
    {
      it "is successful when the input starts with a char in the range"
      {
        p = abr_range("abc");
        t = abr_parse("aaa", 0, p);

        ensure(abr_tree_to_string(t) ===f ""
          "[ null, 1, 0, 1, null, \"range-0\", [] ]");
      }

      it "fails when the input does not start with a char in the range"
      {
        p = abr_range("abc");
        t = abr_parse("daa", 0, p);

        ensure(abr_tree_to_string(t) ===f ""
          "[ null, 0, 0, 0, null, \"range-0\", [] ]");
      }

      it "accepts \"a-z\""
      {
        p = abr_range("a-z");

        t = abr_parse("aaa", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("zaa", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("Aaa", 0, p);

        ensure(t->result == 0);
      }

      it "accepts \"a-z0-9X\""
      {
        p = abr_range("a-z0-9X");

        t = abr_parse("a", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("9", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("X", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("-", 0, p);

        ensure(t->result == 0);
      }

      it "accepts \"\\-\\[\""
      {
        p = abr_range("\\-\\[");

        t = abr_parse("a", 0, p);

        ensure(t->result == 0);

        abr_tree_free(t);
        t = abr_parse("-", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("[", 0, p);

        ensure(t->result == 1);
      }

      it "accepts \"\\--\\[\""
      {
        p = abr_range("\\--\\[");

        t = abr_parse("a", 0, p);

        ensure(t->result == 0);

        abr_tree_free(t);
        t = abr_parse("-", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("[", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("A", 0, p);

        ensure(t->result == 1);
      }

      it "accepts \"^a-z\""
      {
        p = abr_range("^a-z");

        t = abr_parse("a", 0, p);

        ensure(t->result == 0);

        abr_tree_free(t);
        t = abr_parse("A", 0, p);

        ensure(t->result == 1);
      }

      it "accepts \"\\^a-z\""
      {
        p = abr_range("\\^a-z");

        t = abr_parse("a", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("^", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("A", 0, p);

        ensure(t->result == 0);
      }

      it "accepts \"+-\""
      {
        p = abr_range("+-");

        t = abr_parse("+", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("-", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("*", 0, p);

        ensure(t->result == 0);
      }

      it "accepts \".\" (a dot on its own, any char except newline)"
      {
        p = abr_range(".");

        t = abr_parse("a", 0, p);

        ensure(t->result == 1);

        abr_tree_free(t);
        t = abr_parse("\n", 0, p);

        ensure(t->result == 0);

        abr_tree_free(t);
        t = abr_parse("\0", 0, p);

        ensure(t->result == 0);
      }

      it "accepts \"$\" (dollar on its own, the end of the input)"
      {
        p = abr_range("$");

        t = abr_parse("", 0, p);

        ensure(t->result == 1);
        ensure(t->length == 0);

        abr_tree_free(t);
        t = abr_parse("\0", 0, p);

        ensure(t->result == 1);
        ensure(t->length == 0);

        abr_tree_free(t);
        t = abr_parse("a", 0, p);

        ensure(t->result == 0);
        ensure(t->length == 0);
      }
    }
  }
}

