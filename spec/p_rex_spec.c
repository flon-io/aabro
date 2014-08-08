
//
// specifying aabro
//
// Sat Jul 26 06:16:00 JST 2014
//

#include "aabro.h"


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

    it "accepts \"a.b\""
    {
      p = abr_rex("a.b");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq(\n"
        "  abr_string(\"a\"),\n"
        "  abr_range(\".\"),\n"
        "  abr_string(\"b\"),\n"
        "  NULL)");
    }

    it "accepts \"a.?b\""
    {
      p = abr_rex("a.?b");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq(\n"
        "  abr_string(\"a\"),\n"
        "  abr_rep(\n"
        "    abr_range(\".\"), 0, 1),\n"
        "  abr_string(\"b\"),\n"
        "  NULL)");
    }

    it "accepts djan's complex eol rex"
    {
      p = abr_rex("([ \t]*((#[^\r\n]*)?([\r\n]|$))?)*");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_rep(\n"
        "  abr_seq(\n"
        "    abr_rep(\n"
        "      abr_range(\" \t\"), 0, -1),\n"
        "    abr_rep(\n"
        "      abr_seq(\n"
        "        abr_rep(\n"
        "          abr_seq(\n"
        "            abr_string(\"#\"),\n"
        "            abr_rep(\n"
        "              abr_range(\"^\r\n\"), 0, -1),\n"
        "            NULL), 0, 1),\n"
        "        abr_alt(\n"
        "          abr_range(\"\r\n\"),\n"
        "          abr_range(\"$\"),\n"
        "          NULL),\n"
        "        NULL), 0, 1),\n"
        "    NULL), 0, -1)");
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

  describe "abr_parser_to_s(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_rex("a[^b]*b");
      char *s = abr_parser_to_s(p);

      ensure(s ===f "rex t12 \"a[^b]*b\" c1");
    }

    it "returns a string representation of the named parser struct"
    {
      p = abr_n_rex("oedipus", "a[^b]*b");
      char *s = abr_parser_to_s(p);

      ensure(s ===f "rex t12 'oedipus' \"a[^b]*b\" c1");
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

      char *in = "abdef";

      t = abr_parse_f(in, 0, p, ABR_F_ALL);
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
      //p = abr_rex("-?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?");
      p = abr_rex("-?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?");

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

      abr_tree_free(t);
      in = "-1.1e-1234";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f in);

      abr_tree_free(t);
      in = "-1.1e1234X";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f "-1.1e1234");
    }

    it "parses dots"
    {
      p = abr_rex("a.+");

      char *in = NULL;

      //abr_tree_free(t);
      in = "archibald";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f in);

      abr_tree_free(t);
      in = "archived";
      t = abr_parse(in, 0, p);
      //puts(abr_tree_to_string_with_leaves(in, t));
      ensure(t->result == 1);
      ensure(abr_tree_string(in, t) ===f in);
    }

    it "doesn't go into a infinite loop"
    {
      //p = abr_rex("([ \t]*((#[^\r\n]*)?([\r\n]|$))?)*"); // loops
      p = abr_rex("$*"); // loops
      //p = abr_rep(abr_range("$"), 0, -1); // loops
      //p = abr_rex("([ \t]*((#[^\r\n]*)?[\r\n])?)*"); // loops
      //p = abr_rep(abr_string("x"), 0, -1); // doesn't loop
      //p = abr_rex("([ \t]*)*"); // loops
      //p = abr_rep(abr_rep(abr_string("x"), 0, -1), 0, -1); // loops

      t = abr_parse("", 0, p);
      ensure(t->result == 1);
    }
  }
}

