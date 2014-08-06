
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
    abr_tree *t = NULL;
    abr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
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

    it "accepts \"[)(]\""
    {
      p = abr_rex("[)(]");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_range(\")(\")");
    }

    it "accepts \"([)(])\""
    {
      p = abr_rex("([)(])");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_range(\")(\")");
    }

    it "accepts \"(ab)\""
    {
      p = abr_rex("(ab)");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_string(\"ab\")");
    }

    it "accepts \"ab[c-d]ef\""
    {
      p = abr_rex("ab[c-d]ef");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq(\n"
        "  abr_string(\"ab\"),\n"
        "  abr_range(\"c-d\"),\n"
        "  abr_string(\"ef\"),\n"
        "  NULL)");
    }

    it "accepts \"ab|cd\""
    {
      p = abr_rex("ab|cd");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt(\n"
        "  abr_string(\"ab\"),\n"
        "  abr_string(\"cd\"),\n"
        "  NULL)");
    }

    it "accepts \"ab|[a-z|]+|cd\""
    {
      p = abr_rex("ab|[a-z|]+|cd");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt(\n"
        "  abr_string(\"ab\"),\n"
        "  abr_rep(\n"
        "    abr_range(\"a-z|\"), 1, -1),\n"
        "  abr_string(\"cd\"),\n"
        "  NULL)");
    }

    it "accepts \"ab(cd)ef\" (regroups sequences)"
    {
      p = abr_rex("ab(cd)ef");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq(\n"
        "  abr_string(\"ab\"),\n"
        "  abr_string(\"cd\"),\n"
        "  abr_string(\"ef\"),\n"
        "  NULL)");
    }

    it "accepts \"ab(cd)?ef\""
    {
      p = abr_rex("ab(cd)?ef");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq(\n"
        "  abr_string(\"ab\"),\n"
        "  abr_rep(\n"
        "    abr_string(\"cd\"), 0, 1),\n"
        "  abr_string(\"ef\"),\n"
        "  NULL)");
    }

    it "accepts \"ab|(cd|ef)|gh\" (regroups alternatives)"
    {
      p = abr_rex("ab|(cd|ef)|gh");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt(\n"
        "  abr_string(\"ab\"),\n"
        "  abr_string(\"cd\"),\n"
        "  abr_string(\"ef\"),\n"
        "  abr_string(\"gh\"),\n"
        "  NULL)");
    }

    it "accepts \"ab|(cd[a-z\\(])ef\""
    {
      p = abr_rex("ab|(cd[a-z\\(])ef");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt(\n"
        "  abr_string(\"ab\"),\n"
        "  abr_seq(\n"
        "    abr_string(\"cd\"),\n"
        "    abr_range(\"a-z\\(\"),\n"
        "    abr_string(\"ef\"),\n"
        "    NULL),\n"
        "  NULL)");
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

  context "parsing"
  {
    it "groups successful results"
    {
      p = abr_rex("ab[c-d]ef");

      char *in = "abdef";

      t = abr_parse(in, 0, p);
      char *s = abr_tree_to_string_with_leaves(in, t);

      ensure(s ===f ""
        "[ null, 1, 0, 5, null, \"rex\", \"abdef\" ]");
    }

    it "doesn't group when prune == 0"
    {
      p = abr_rex("ab[c-d]ef");

      abr_conf c = { .prune = 0, .all = 1 };

      char *in = "abdef";

      t = abr_parse_c(in, 0, p, c);
      char *s = abr_tree_to_string_with_leaves(in, t);

      ensure(s ===f ""
        "[ null, 1, 0, 5, null, \"rex\", [\n"
        "  [ null, 1, 0, 5, null, \"seq\", [\n"
        "    [ null, 1, 0, 2, null, \"string\", \"ab\" ],\n"
        "    [ null, 1, 2, 1, null, \"range\", \"d\" ],\n"
        "    [ null, 1, 3, 2, null, \"string\", \"ef\" ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "parses numbers"
    {
      //p = abr_regex("^-?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?");
      //p = abr_rex("-?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?");
      p = abr_rex("-?[0-9]+(.[0-9]+)?([eE][+-]?[0-9]+)?");

      char *in = NULL;

      //abr_tree_free(t);
      in = "1";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f in);

      abr_tree_free(t);
      in = "-1";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f in);

      abr_tree_free(t);
      in = "1.1";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f in);

      abr_tree_free(t);
      in = "-1.1";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f in);

      abr_tree_free(t);
      in = "-1.1e1234";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f in);

      // argh, parse all ?

      abr_tree_free(t);
      in = "-1.1e1234X";
      t = abr_parse(in, 0, p);
      puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 0);
    }
  }
}

