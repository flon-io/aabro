
//
// specifying aabro
//
// Sat Jul 26 06:16:00 JST 2014
//

#include "aabro.h"


describe "fabr_rex()"
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

  it "accepts \"a\" (failure)"
  {
    i.string = "bravo";
    t = fabr_rex("x", &i, "a");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 1, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 1, [\n"
      "    [ null, 0, 0, 0, null, \"str\", 1, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"a\" (success)"
  {
    i.string = "alpha";
    t = fabr_rex("x", &i, "a");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 1, null, \"rex_alt\", 1, [\n"
      "  [ null, 1, 0, 1, null, \"rex_seq\", 1, [\n"
      "    [ null, 1, 0, 1, null, \"str\", 1, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab|cd\" (failure)"
  {
    i.string = "acadabra";
    t = fabr_rex("x", &i, "ab|cd");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 5, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 2, [\n"
      "    [ null, 0, 0, 0, null, \"str\", 2, [] ]\n"
      "  ] ],\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 2, [\n"
      "    [ null, 0, 0, 0, null, \"str\", 2, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab|cd\" (success)"
  {
    i.string = "abdera";
    t = fabr_rex("x", &i, "ab|cd");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex_alt\", 5, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 2, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab|cd\" (success 2)"
  {
    i.string = "cdhulu";
    t = fabr_rex("x", &i, "ab|cd");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex_alt\", 5, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 2, [\n"
      "    [ null, 0, 0, 0, null, \"str\", 2, [] ]\n"
      "  ] ],\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 2, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"abc+\" (failure)"
  {
    i.string = "abd";
    t = fabr_rex("x", &i, "abc+");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 4, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 4, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 0, 2, 0, null, \"rex_rep\", 2, [\n"
      "      [ null, 0, 2, 0, null, \"str\", 1, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"abc+\" (success)"
  {
    i.string = "abcc";
    t = fabr_rex("x", &i, "abc+");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex_alt\", 4, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 4, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 1, 2, 2, null, \"rex_rep\", 2, [\n"
      "      [ null, 1, 2, 1, null, \"str\", 1, [] ],\n"
      "      [ null, 1, 3, 1, null, \"str\", 1, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]\" (failure)"
  {
    i.string = "abe";
    t = fabr_rex("x", &i, "ab[cd]");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 6, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 6, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 0, 2, 0, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]\" (success)"
  {
    i.string = "abc";
    t = fabr_rex("x", &i, "ab[cd]");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex_alt\", 6, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 6, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 1, 2, 1, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]ef\" (success)"
  {
    i.string = "abcef";
    t = fabr_rex("x", &i, "ab[cd]ef");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 5, null, \"rex_alt\", 8, [\n"
      "  [ null, 1, 0, 5, null, \"rex_seq\", 8, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 1, 2, 1, null, \"rng\", 4, [] ],\n"
      "    [ null, 1, 3, 2, null, \"str\", 2, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]cd\" (failure)"
  {
    i.string = "ccd";
    t = fabr_rex("x", &i, "[ab]cd");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 6, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 6, [\n"
      "    [ null, 0, 0, 0, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]cd\" (success)"
  {
    i.string = "acd";
    t = fabr_rex("x", &i, "[ab]cd");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex_alt\", 6, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 6, [\n"
      "    [ null, 1, 0, 1, null, \"rng\", 4, [] ],\n"
      "    [ null, 1, 1, 2, null, \"str\", 2, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]+\" (failure)"
  {
    i.string = "abe";
    t = fabr_rex("x", &i, "ab[cd]+");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 7, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 0, 2, 0, null, \"rex_rep\", 5, [\n"
      "      [ null, 0, 2, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]+\" (success)"
  {
    i.string = "abc";
    t = fabr_rex("x", &i, "ab[cd]+");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex_alt\", 7, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 1, 2, 1, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]+\" (success 2)"
  {
    i.string = "abcd";
    t = fabr_rex("x", &i, "ab[cd]+");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex_alt\", 7, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 1, 2, 2, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, [] ],\n"
      "      [ null, 1, 3, 1, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]*\" (failure)"
  {
    i.string = "abe";
    t = fabr_rex("x", &i, "ab[cd]*");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex_alt\", 7, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 1, 2, 0, null, \"rex_rep\", 5, [\n"
      "      [ null, 0, 2, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]*\" (success)"
  {
    i.string = "ab";
    t = fabr_rex("x", &i, "ab[cd]*");

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex_alt\", 7, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, \"ab\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]*\" (success 2)"
  {
    i.string = "abcd";
    t = fabr_rex("x", &i, "ab[cd]*");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex_alt\", 7, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, [] ],\n"
      "    [ null, 1, 2, 2, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, [] ],\n"
      "      [ null, 1, 3, 1, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]\" (failure)"
  {
    i.string = "catapult";
    t = fabr_rex("x", &i, "[ab]");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 4, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 4, [\n"
      "    [ null, 0, 0, 0, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]\" (success)"
  {
    i.string = "arles";
    t = fabr_rex("x", &i, "[ab]");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 1, null, \"rex_alt\", 4, [\n"
      "  [ null, 1, 0, 1, null, \"rex_seq\", 4, [\n"
      "    [ null, 1, 0, 1, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]+\" (failure)"
  {
    i.string = "carl";
    t = fabr_rex("x", &i, "[ab]+");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 5, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 5, [\n"
      "    [ null, 0, 0, 0, null, \"rex_rep\", 5, [\n"
      "      [ null, 0, 0, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]+\" (success)"
  {
    i.string = "aba";
    t = fabr_rex("x", &i, "[ab]+");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex_alt\", 5, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 5, [\n"
      "    [ null, 1, 0, 3, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, [] ],\n"
      "      [ null, 1, 1, 1, null, \"rng\", 2, [] ],\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]?\" (success)"
  {
    i.string = "c";
    t = fabr_rex("x", &i, "[ab]?");

    ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 0, null, \"rex_alt\", 5, [\n"
      "  [ null, 1, 0, 0, null, \"rex_seq\", 5, [\n"
      "    [ null, 1, 0, 0, null, \"rex_rep\", 5, [\n"
      "      [ null, 0, 0, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]?\" (success 2)"

  it "accepts \"[ab]*\" (failure)"
  it "accepts \"[ab]*\" (success)"
  it "accepts \"[ab]{2}\" (failure)"
  it "accepts \"[ab]{2}\" (success)"
  it "accepts \"[ab]{2,3}\" (failure)"
  it "accepts \"[ab]{2,3}\" (success)"

  it "accepts \"ab(cd|ef)\" (failure)"
  {
    i.string = "abgh";
    t = fabr_rex("x", &i, "ab(cd|ef)");

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex_alt\", 9, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, \"ab\" ],\n"
      "    [ null, 0, 2, 0, null, \"rex_alt\", 7, [\n"
      "      [ null, 0, 2, 0, null, \"rex_seq\", 2, [\n"
      "        [ null, 0, 2, 0, null, \"str\", 2, [] ]\n"
      "      ] ],\n"
      "      [ null, 0, 2, 0, null, \"rex_seq\", 2, [\n"
      "        [ null, 0, 2, 0, null, \"str\", 2, [] ]\n"
      "      ] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab(cd|ef)\" (success)"
  {
    i.string = "abef";
    t = fabr_rex("x", &i, "ab(cd|ef)");

    ensure(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex_alt\", 9, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 2, null, \"str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 2, null, \"rex_alt\", 7, [\n"
      "      [ null, 0, 2, 0, null, \"rex_seq\", 2, [\n"
      "        [ null, 0, 2, 0, null, \"str\", 2, [] ]\n"
      "      ] ],\n"
      "      [ null, 1, 2, 2, null, \"rex_seq\", 2, [\n"
      "        [ null, 1, 2, 2, null, \"str\", 2, \"ef\" ]\n"
      "      ] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  context "errors"
  {
    it "rejects \"[ab\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "[ab");

      ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex_alt\", 3, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 3, [\n"
        "    [ null, -1, 0, 0, \"range not closed >[ab<3\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"a{1,2fda\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "a{1,2fda");

      ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex_alt\", 8, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 8, [\n"
        "    [ null, -1, 0, 0, \"invalid {min[,max]} >a{1,2fda<8\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"a{1,2fda}nada\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "a{1,2fda}nada");

      ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex_alt\", 13, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 13, [\n"
        "    [ null, -1, 0, 0, \"invalid {min[,max]} >a{1,2fda}nada<13\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"a{1,2,3}nada\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "a{1,2,3}nada");

      ensure(fabr_tree_to_string(t, NULL, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex_alt\", 12, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 12, [\n"
        "    [ null, -1, 0, 0, \"invalid {min[,max]} >a{1,2,3}nada<12\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }
  }
}

//  context "parsing"
//  {
//    it "groups successful results"
//    {
//      p = fabr_rex("ab[c-d]ef");
//
//      char *in = "abdef";
//
//      t = fabr_parse(in, 0, p);
//      char *s = fabr_tree_to_string(t, in, 0);
//
//      ensure(s ===f ""
//        "[ null, 1, 0, 5, null, \"rex-0\", \"abdef\" ]");
//    }
//
//    it "doesn't group when prune == 0"
//    {
//      p = fabr_rex("ab[c-d]ef");
//
//      char *in = "abdef";
//
//      t = fabr_parse_f(in, 0, p, FABR_F_ALL);
//      char *s = fabr_tree_to_string(t, in, 0);
//
//      ensure(s ===f ""
//        "[ null, 1, 0, 5, null, \"rex-0\", [\n"
//        "  [ null, 1, 0, 5, null, \"seq-00\", [\n"
//        "    [ null, 1, 0, 2, null, \"string-000\", \"ab\" ],\n"
//        "    [ null, 1, 2, 1, null, \"range-001\", \"d\" ],\n"
//        "    [ null, 1, 3, 2, null, \"string-002\", \"ef\" ]\n"
//        "  ] ]\n"
//        "] ]");
//    }
//
//    it "parses numbers"
//    {
//      //p = fabr_rex("-?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?");
//      //p = fabr_rex("-?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?");
//      p = fabr_rex("-?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?");
//
//      char *in = NULL;
//
//      //fabr_tree_free(t);
//      in = "1";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in, 1));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f in);
//
//      fabr_tree_free(t);
//      in = "-1";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in, 1));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f in);
//
//      fabr_tree_free(t);
//      in = "1.1";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in, 1));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f in);
//
//      fabr_tree_free(t);
//      in = "-1.1";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in, 1));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f in);
//
//      fabr_tree_free(t);
//      in = "-1.1e1234";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in, 1));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f in);
//
//      fabr_tree_free(t);
//      in = "-1.1e-1234";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in, 1));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f in);
//
//      fabr_tree_free(t);
//      in = "-1.1e1234X";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in, 1));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f "-1.1e1234");
//    }
//
//    it "parses dots"
//    {
//      p = fabr_rex("a.+");
//
//      char *in = NULL;
//
//      //fabr_tree_free(t);
//      in = "archibald";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f in);
//
//      fabr_tree_free(t);
//      in = "archived";
//      t = fabr_parse(in, 0, p);
//      //puts(fabr_tree_to_string(t, in));
//      ensure(t->result == 1);
//      ensure(fabr_tree_string(in, t) ===f in);
//    }
//
//    it "doesn't go into a infinite loop"
//    {
//      //p = fabr_rex("([ \t]*((#[^\r\n]*)?([\r\n]|$))?)*"); // loops
//      p = fabr_rex("$*"); // loops
//      //p = fabr_rep(fabr_range("$"), 0, -1); // loops
//      //p = fabr_rex("([ \t]*((#[^\r\n]*)?[\r\n])?)*"); // loops
//      //p = fabr_rep(fabr_string("x"), 0, -1); // doesn't loop
//      //p = fabr_rex("([ \t]*)*"); // loops
//      //p = fabr_rep(fabr_rep(fabr_string("x"), 0, -1), 0, -1); // loops
//
//      t = fabr_parse("", 0, p);
//      ensure(t->result == 1);
//    }
//
//    it "parses ranges with escapes"
//    {
//      p = fabr_rex("[\\]]");
//
//      t = fabr_parse("a", 0, p);
//      ensure(t->result == 0);
//
//      fabr_tree_free(t);
//      t = fabr_parse("]", 0, p);
//      ensure(t->result == 1);
//    }

//    it "accepts \"a\""
//    {
//      p = fabr_rex("a");
//
//      ensure(fabr_parser_to_string(p) ===f ""
//        "fabr_rex(\"a\") /* 0 */");
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_string(\"a\") /* 00 */");
//    }
//
//    it "accepts \"a\\?\""
//    {
//      p = fabr_rex("a\\?");
//
//      ensure(fabr_parser_to_string(p) ===f ""
//        "fabr_rex(\"a\\?\") /* 0 */");
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_string(\"a?\") /* 00 */");
//    }
//
//    it "accepts \"a+\""
//    {
//      p = fabr_rex("a+");
//
//      ensure(fabr_parser_to_string(p) ===f ""
//        "fabr_rex(\"a+\") /* 0 */");
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_rep( /* 00 */\n"
//        "  fabr_string(\"a\") /* 000 */, 1, -1)");
//    }
//
//    it "accepts \"ab+\""
//    {
//      p = fabr_rex("ab+");
//
//      ensure(fabr_parser_to_string(p) ===f ""
//        "fabr_rex(\"ab+\") /* 0 */");
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_seq( /* 00 */\n"
//        "  fabr_string(\"a\") /* 000 */,\n"
//        "  fabr_rep( /* 001 */\n"
//        "    fabr_string(\"b\") /* 0010 */, 1, -1),\n"
//        "  NULL)");
//    }
//
//    it "accepts \"ab+c\""
//    {
//      p = fabr_rex("ab+c");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_seq( /* 0 */\n"
//        "  fabr_string(\"a\") /* 00 */,\n"
//        "  fabr_rep( /* 01 */\n"
//        "    fabr_string(\"b\") /* 010 */, 1, -1),\n"
//        "  fabr_string(\"c\") /* 02 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"[a-z]+\""
//    {
//      p = fabr_rex("[a-z]+");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_rep( /* 0 */\n"
//        "  fabr_range(\"a-z\") /* 00 */, 1, -1)");
//    }
//
//    it "accepts \"[)(]\""
//    {
//      p = fabr_rex("[)(]");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_range(\")(\") /* 0 */");
//    }
//
//    it "accepts \"([)(])\""
//    {
//      p = fabr_rex("([)(])");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_range(\")(\") /* 0 */");
//    }
//
//    it "accepts \"(ab)\""
//    {
//      p = fabr_rex("(ab)");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_string(\"ab\") /* 0 */");
//    }
//
//    it "accepts \"ab[c-d]ef\""
//    {
//      p = fabr_rex("ab[c-d]ef");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_seq( /* 0 */\n"
//        "  fabr_string(\"ab\") /* 00 */,\n"
//        "  fabr_range(\"c-d\") /* 01 */,\n"
//        "  fabr_string(\"ef\") /* 02 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"ab|cd\""
//    {
//      p = fabr_rex("ab|cd");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_alt( /* 0 */\n"
//        "  fabr_string(\"ab\") /* 00 */,\n"
//        "  fabr_string(\"cd\") /* 01 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"ab|[a-z|]+|cd\""
//    {
//      p = fabr_rex("ab|[a-z|]+|cd");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_alt( /* 0 */\n"
//        "  fabr_string(\"ab\") /* 00 */,\n"
//        "  fabr_rep( /* 01 */\n"
//        "    fabr_range(\"a-z|\") /* 010 */, 1, -1),\n"
//        "  fabr_string(\"cd\") /* 02 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"ab(cd)ef\" (regroups sequences)"
//    {
//      p = fabr_rex("ab(cd)ef");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_seq( /* 0 */\n"
//        "  fabr_string(\"ab\") /* 00 */,\n"
//        "  fabr_string(\"cd\") /* 01 */,\n"
//        "  fabr_string(\"ef\") /* 02 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"ab(cd)?ef\""
//    {
//      p = fabr_rex("ab(cd)?ef");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_seq( /* 0 */\n"
//        "  fabr_string(\"ab\") /* 00 */,\n"
//        "  fabr_rep( /* 01 */\n"
//        "    fabr_string(\"cd\") /* 010 */, 0, 1),\n"
//        "  fabr_string(\"ef\") /* 02 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"ab|(cd|ef)|gh\" (regroups alternatives)"
//    {
//      p = fabr_rex("ab|(cd|ef)|gh");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_alt( /* 0 */\n"
//        "  fabr_string(\"ab\") /* 00 */,\n"
//        "  fabr_string(\"cd\") /* 01 */,\n"
//        "  fabr_string(\"ef\") /* 02 */,\n"
//        "  fabr_string(\"gh\") /* 03 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"ab|(cd[a-z\\(])ef\""
//    {
//      p = fabr_rex("ab|(cd[a-z\\(])ef");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_alt( /* 0 */\n"
//        "  fabr_string(\"ab\") /* 00 */,\n"
//        "  fabr_seq( /* 01 */\n"
//        "    fabr_string(\"cd\") /* 010 */,\n"
//        "    fabr_range(\"a-z\\(\") /* 011 */,\n"
//        "    fabr_string(\"ef\") /* 012 */,\n"
//        "    NULL),\n"
//        "  NULL)");
//    }
//
//    it "accepts \"ab|(cd[a-z\\)])ef\""
//    {
//      p = fabr_rex("ab|(cd[a-z\\)])ef");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_alt( /* 0 */\n"
//        "  fabr_string(\"ab\") /* 00 */,\n"
//        "  fabr_seq( /* 01 */\n"
//        "    fabr_string(\"cd\") /* 010 */,\n"
//        "    fabr_range(\"a-z\\)\") /* 011 */,\n"
//        "    fabr_string(\"ef\") /* 012 */,\n"
//        "    NULL),\n"
//        "  NULL)");
//    }
//
//    it "accepts \"a.b\""
//    {
//      p = fabr_rex("a.b");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_seq( /* 0 */\n"
//        "  fabr_string(\"a\") /* 00 */,\n"
//        "  fabr_range(\".\") /* 01 */,\n"
//        "  fabr_string(\"b\") /* 02 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"a.?b\""
//    {
//      p = fabr_rex("a.?b");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_seq( /* 0 */\n"
//        "  fabr_string(\"a\") /* 00 */,\n"
//        "  fabr_rep( /* 01 */\n"
//        "    fabr_range(\".\") /* 010 */, 0, 1),\n"
//        "  fabr_string(\"b\") /* 02 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts djan's complex eol rex"
//    {
//      p = fabr_rex("([ \t]*((#[^\r\n]*)?([\r\n]|$))?)*");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_rep( /* 0 */\n"
//        "  fabr_seq( /* 00 */\n"
//        "    fabr_rep( /* 000 */\n"
//        "      fabr_range(\" \t\") /* 0000 */, 0, -1),\n"
//        "    fabr_rep( /* 001 */\n"
//        "      fabr_seq( /* 0010 */\n"
//        "        fabr_rep( /* 00100 */\n"
//        "          fabr_seq( /* 001000 */\n"
//        "            fabr_string(\"#\") /* 0010000 */,\n"
//        "            fabr_rep( /* 0010001 */\n"
//        "              fabr_range(\"^\r\n\") /* 00100010 */, 0, -1),\n"
//        "            NULL), 0, 1),\n"
//        "        fabr_alt( /* 00101 */\n"
//        "          fabr_range(\"\r\n\") /* 001010 */,\n"
//        "          fabr_range(\"$\") /* 001011 */,\n"
//        "          NULL),\n"
//        "        NULL), 0, 1),\n"
//        "    NULL), 0, -1)");
//    }
//
//    it "accepts \"a[0-9]{4}|b\""
//    {
//      p = fabr_rex("a[0-9]{4}|b");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_alt( /* 0 */\n"
//        "  fabr_seq( /* 00 */\n"
//        "    fabr_string(\"a\") /* 000 */,\n"
//        "    fabr_rep( /* 001 */\n"
//        "      fabr_range(\"0-9\") /* 0010 */, 4, 4),\n"
//        "    NULL),\n"
//        "  fabr_string(\"b\") /* 01 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"a[0-9]{4,5}|b\""
//    {
//      p = fabr_rex("a[0-9]{4,5}|b");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_alt( /* 0 */\n"
//        "  fabr_seq( /* 00 */\n"
//        "    fabr_string(\"a\") /* 000 */,\n"
//        "    fabr_rep( /* 001 */\n"
//        "      fabr_range(\"0-9\") /* 0010 */, 4, 5),\n"
//        "    NULL),\n"
//        "  fabr_string(\"b\") /* 01 */,\n"
//        "  NULL)");
//    }
//
//    it "accepts \"[\"\\]]+\""
//    {
//      p = fabr_rex("[\"\\]]+");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_rep( /* 0 */\n"
//        "  fabr_range(\"\"\\]\") /* 00 */, 1, -1)");
//    }
//
//    it "accepts \"(\\(\\)|\\)\\()\""
//    {
//      p = fabr_rex("(\\(\\)|\\)\\()");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_alt( /* 0 */\n"
//        "  fabr_string(\"()\") /* 00 */,\n"
//        "  fabr_string(\")(\") /* 01 */,\n"
//        "  NULL)");
//    }
//  }
//
//  describe "fabr_n_rex(name, s)"
//  {
//    it "creates a named rex parsing tree"
//    {
//      p = fabr_n_rex("a_and_bs", "ab+");
//
//      ensure(fabr_parser_to_string(p) ===f ""
//        "fabr_n_rex(\"a_and_bs\", \"ab+\") /* 0 */");
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_seq( /* 00 */\n"
//        "  fabr_string(\"a\") /* 000 */,\n"
//        "  fabr_rep( /* 001 */\n"
//        "    fabr_string(\"b\") /* 0010 */, 1, -1),\n"
//        "  NULL)");
//    }
//  }
//
//  context "errors"
//  {
//    it "fails when a quantifier is orphan"
//    {
//      p = fabr_rex("{1,2}");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_error(\"orphan quantifier >{1,2}<\") /* 0 */");
//    }
//
//    it "fails when a range isn't closed"
//    {
//      p = fabr_rex("[a-z");
//
//      ensure(fabr_parser_to_string(p->children[0]) ===f ""
//        "fabr_error(\"range not closed >[a-z<\") /* 0 */");
//    }
//  }
//
//  describe "fabr_parser_to_s(p)"
//  {
//    it "returns a string representation of the parser struct"
//    {
//      p = fabr_rex("a[^b]*b");
//      char *s = fabr_parser_to_s(p);
//
//      ensure(s ===f "rex t13 \"a[^b]*b\" c1");
//    }
//
//    it "returns a string representation of the named parser struct"
//    {
//      p = fabr_n_rex("oedipus", "a[^b]*b");
//      char *s = fabr_parser_to_s(p);
//
//      ensure(s ===f "rex t13 'oedipus' \"a[^b]*b\" c1");
//    }
//  }

