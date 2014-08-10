
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
        "abr_rex(\"a\") /* 0 */");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_string(\"a\") /* 00 */");
    }

    it "accepts \"a\\?\""
    {
      p = abr_rex("a\\?");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_rex(\"a\\?\") /* 0 */");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_string(\"a?\") /* 00 */");
    }

    it "accepts \"a+\""
    {
      p = abr_rex("a+");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_rex(\"a+\") /* 0 */");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_rep( /* 00 */\n"
        "  abr_string(\"a\") /* 000 */, 1, -1)");
    }

    it "accepts \"ab+\""
    {
      p = abr_rex("ab+");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_rex(\"ab+\") /* 0 */");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq( /* 00 */\n"
        "  abr_string(\"a\") /* 000 */,\n"
        "  abr_rep( /* 001 */\n"
        "    abr_string(\"b\") /* 0010 */, 1, -1),\n"
        "  NULL)");
    }

    it "accepts \"ab+c\""
    {
      p = abr_rex("ab+c");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_string(\"a\") /* 00 */,\n"
        "  abr_rep( /* 01 */\n"
        "    abr_string(\"b\") /* 010 */, 1, -1),\n"
        "  abr_string(\"c\") /* 02 */,\n"
        "  NULL)");
    }

    it "accepts \"[a-z]+\""
    {
      p = abr_rex("[a-z]+");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_rep( /* 0 */\n"
        "  abr_range(\"a-z\") /* 00 */, 1, -1)");
    }

    it "accepts \"[)(]\""
    {
      p = abr_rex("[)(]");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_range(\")(\") /* 0 */");
    }

    it "accepts \"([)(])\""
    {
      p = abr_rex("([)(])");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_range(\")(\") /* 0 */");
    }

    it "accepts \"(ab)\""
    {
      p = abr_rex("(ab)");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_string(\"ab\") /* 0 */");
    }

    it "accepts \"ab[c-d]ef\""
    {
      p = abr_rex("ab[c-d]ef");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_string(\"ab\") /* 00 */,\n"
        "  abr_range(\"c-d\") /* 01 */,\n"
        "  abr_string(\"ef\") /* 02 */,\n"
        "  NULL)");
    }

    it "accepts \"ab|cd\""
    {
      p = abr_rex("ab|cd");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt( /* 0 */\n"
        "  abr_string(\"ab\") /* 00 */,\n"
        "  abr_string(\"cd\") /* 01 */,\n"
        "  NULL)");
    }

    it "accepts \"ab|[a-z|]+|cd\""
    {
      p = abr_rex("ab|[a-z|]+|cd");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt( /* 0 */\n"
        "  abr_string(\"ab\") /* 00 */,\n"
        "  abr_rep( /* 01 */\n"
        "    abr_range(\"a-z|\") /* 010 */, 1, -1),\n"
        "  abr_string(\"cd\") /* 02 */,\n"
        "  NULL)");
    }

    it "accepts \"ab(cd)ef\" (regroups sequences)"
    {
      p = abr_rex("ab(cd)ef");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_string(\"ab\") /* 00 */,\n"
        "  abr_string(\"cd\") /* 01 */,\n"
        "  abr_string(\"ef\") /* 02 */,\n"
        "  NULL)");
    }

    it "accepts \"ab(cd)?ef\""
    {
      p = abr_rex("ab(cd)?ef");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_string(\"ab\") /* 00 */,\n"
        "  abr_rep( /* 01 */\n"
        "    abr_string(\"cd\") /* 010 */, 0, 1),\n"
        "  abr_string(\"ef\") /* 02 */,\n"
        "  NULL)");
    }

    it "accepts \"ab|(cd|ef)|gh\" (regroups alternatives)"
    {
      p = abr_rex("ab|(cd|ef)|gh");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt( /* 0 */\n"
        "  abr_string(\"ab\") /* 00 */,\n"
        "  abr_string(\"cd\") /* 01 */,\n"
        "  abr_string(\"ef\") /* 02 */,\n"
        "  abr_string(\"gh\") /* 03 */,\n"
        "  NULL)");
    }

    it "accepts \"ab|(cd[a-z\\(])ef\""
    {
      p = abr_rex("ab|(cd[a-z\\(])ef");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt( /* 0 */\n"
        "  abr_string(\"ab\") /* 00 */,\n"
        "  abr_seq( /* 01 */\n"
        "    abr_string(\"cd\") /* 010 */,\n"
        "    abr_range(\"a-z\\(\") /* 011 */,\n"
        "    abr_string(\"ef\") /* 012 */,\n"
        "    NULL),\n"
        "  NULL)");
    }

    it "accepts \"a.b\""
    {
      p = abr_rex("a.b");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_string(\"a\") /* 00 */,\n"
        "  abr_range(\".\") /* 01 */,\n"
        "  abr_string(\"b\") /* 02 */,\n"
        "  NULL)");
    }

    it "accepts \"a.?b\""
    {
      p = abr_rex("a.?b");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_string(\"a\") /* 00 */,\n"
        "  abr_rep( /* 01 */\n"
        "    abr_range(\".\") /* 010 */, 0, 1),\n"
        "  abr_string(\"b\") /* 02 */,\n"
        "  NULL)");
    }

    it "accepts djan's complex eol rex"
    {
      p = abr_rex("([ \t]*((#[^\r\n]*)?([\r\n]|$))?)*");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_rep( /* 0 */\n"
        "  abr_seq( /* 00 */\n"
        "    abr_rep( /* 000 */\n"
        "      abr_range(\" \t\") /* 0000 */, 0, -1),\n"
        "    abr_rep( /* 001 */\n"
        "      abr_seq( /* 0010 */\n"
        "        abr_rep( /* 00100 */\n"
        "          abr_seq( /* 001000 */\n"
        "            abr_string(\"#\") /* 0010000 */,\n"
        "            abr_rep( /* 0010001 */\n"
        "              abr_range(\"^\r\n\") /* 00100010 */, 0, -1),\n"
        "            NULL), 0, 1),\n"
        "        abr_alt( /* 00101 */\n"
        "          abr_range(\"\r\n\") /* 001010 */,\n"
        "          abr_range(\"$\") /* 001011 */,\n"
        "          NULL),\n"
        "        NULL), 0, 1),\n"
        "    NULL), 0, -1)");
    }

    it "accepts \"a[0-9]{4}|b\""
    {
      p = abr_rex("a[0-9]{4}|b");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt( /* 0 */\n"
        "  abr_seq( /* 00 */\n"
        "    abr_string(\"a\") /* 000 */,\n"
        "    abr_rep( /* 001 */\n"
        "      abr_range(\"0-9\") /* 0010 */, 4, 4),\n"
        "    NULL),\n"
        "  abr_string(\"b\") /* 01 */,\n"
        "  NULL)");
    }

    it "accepts \"a[0-9]{4,5}|b\""
    {
      p = abr_rex("a[0-9]{4,5}|b");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_alt( /* 0 */\n"
        "  abr_seq( /* 00 */\n"
        "    abr_string(\"a\") /* 000 */,\n"
        "    abr_rep( /* 001 */\n"
        "      abr_range(\"0-9\") /* 0010 */, 4, 5),\n"
        "    NULL),\n"
        "  abr_string(\"b\") /* 01 */,\n"
        "  NULL)");
    }
  }

  describe "abr_n_rex(name, s)"
  {
    it "creates a named rex parsing tree"
    {
      p = abr_n_rex("a_and_bs", "ab+");

      ensure(abr_parser_to_string(p) ===f ""
        "abr_n_rex(\"a_and_bs\", \"ab+\") /* 0 */");
      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_seq( /* 00 */\n"
        "  abr_string(\"a\") /* 000 */,\n"
        "  abr_rep( /* 001 */\n"
        "    abr_string(\"b\") /* 0010 */, 1, -1),\n"
        "  NULL)");
    }
  }

  context "errors"
  {
    it "fails when a quantifier is orphan"
    {
      p = abr_rex("{1,2}");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_error(\"orphan quantifier >{1,2}<\") /* 0 */");
    }

    it "fails when a range isn't closed"
    {
      p = abr_rex("[a-z");

      ensure(abr_parser_to_string(p->children[0]) ===f ""
        "abr_error(\"range not closed >[a-z<\") /* 0 */");
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
        "[ null, 1, 0, 5, null, \"rex-0\", \"abdef\" ]");
    }

    it "doesn't group when prune == 0"
    {
      p = abr_rex("ab[c-d]ef");

      char *in = "abdef";

      t = abr_parse_f(in, 0, p, ABR_F_ALL);
      char *s = abr_tree_to_string_with_leaves(in, t);

      ensure(s ===f ""
        "[ null, 1, 0, 5, null, \"rex-0\", [\n"
        "  [ null, 1, 0, 5, null, \"seq-00\", [\n"
        "    [ null, 1, 0, 2, null, \"string-000\", \"ab\" ],\n"
        "    [ null, 1, 2, 1, null, \"range-001\", \"d\" ],\n"
        "    [ null, 1, 3, 2, null, \"string-002\", \"ef\" ]\n"
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

