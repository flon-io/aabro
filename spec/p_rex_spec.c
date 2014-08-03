
//
// specifying aabro
//
// Sat Jul 26 06:16:00 JST 2014
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
          "[ null, 1, 0, 1, null, \"range\", [] ]");
      }

      it "fails when the input does not start with a char in the range"
      {
        p = abr_range("abc");
        t = abr_parse("daa", 0, p);

        ensure(abr_tree_to_string(t) ===f ""
          "[ null, 0, 0, 0, null, \"range\", [] ]");
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
    }
  }
}

context "abr_rex"
{
  before each
  {
    abr_parser *p = NULL;
  }
  after each
  {
    if (p != NULL) abr_parser_free(p);
  }

  describe "abr_rex(s)"
  {
    it "accepts \"a\""
    {
      p = abr_rex("a");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_rex(\"a\")");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_string(\"a\")");
    }

    it "accepts \"a\\?\""
    {
      p = abr_rex("a\\?");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_rex(\"a\\?\")");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_string(\"a?\")");
    }

    it "accepts \"a+\""
    {
      p = abr_rex("a+");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_rex(\"a+\")");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_rep(\n"
        "  abr_string(\"a\"), 1, -1)");
    }

    it "accepts \"ab+\""
    {
      p = abr_rex("ab+");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_rex(\"ab+\")");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq(\n"
        "  abr_string(\"a\"),\n"
        "  abr_rep(\n"
        "    abr_string(\"b\"), 1, -1),\n"
        "  NULL)");
    }

    it "accepts \"ab+c\""
    {
      p = abr_rex("ab+c");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq(\n"
        "  abr_string(\"a\"),\n"
        "  abr_rep(\n"
        "    abr_string(\"b\"), 1, -1),\n"
        "  abr_string(\"c\"),\n"
        "  NULL)");
    }

    it "accepts \"[a-z]+\""
    {
      p = abr_rex("[a-z]+");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_rep(\n"
        "  abr_range(\"a-z\"), 1, -1)");
    }
  }

  describe "abr_n_rex(name, s)"
  {
    it "creates a named rex parsing tree"
    {
      p = abr_n_rex("a_and_bs", "ab+");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_n_rex(\"a_and_bs\", \"ab+\")");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq(\n"
        "  abr_string(\"a\"),\n"
        "  abr_rep(\n"
        "    abr_string(\"b\"), 1, -1),\n"
        "  NULL)");
    }
  }

  context "errors"
  {
    it "fails when a quantifier is orphan"
    {
      p = abr_rex("{1,2}");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_error(\"orphan quantifier >{1,2}<\")");
    }

    it "fails when a range isn't closed"
    {
      p = abr_rex("[a-z");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_error(\"range not closed >[a-z<\")");
    }
  }
}

