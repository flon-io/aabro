
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
    fabr_input i = { "", 0, 0 };
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

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 1, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 1, [\n"
      "    [ null, 0, 0, 0, null, \"rex_str\", 1, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"a\" (success)"
  {
    i.string = "alpha";
    t = fabr_rex("x", &i, "a");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 1, null, \"rex\", 1, [\n"
      "  [ null, 1, 0, 1, null, \"rex_seq\", 1, [\n"
      "    [ null, 1, 0, 1, null, \"rex_str\", 1, \"a\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab|cd\" (failure)"
  {
    i.string = "acadabra";
    t = fabr_rex("x", &i, "ab|cd");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 5, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 2, [\n"
      "    [ null, 0, 0, 0, null, \"rex_str\", 2, [] ]\n"
      "  ] ],\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 2, [\n"
      "    [ null, 0, 0, 0, null, \"rex_str\", 2, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab|cd\" (success)"
  {
    i.string = "abdera";
    t = fabr_rex("x", &i, "ab|cd");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex\", 5, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 2, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab|cd\" (success 2)"
  {
    i.string = "cdhulu";
    t = fabr_rex("x", &i, "ab|cd");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex\", 5, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 2, [\n"
      "    [ null, 0, 0, 0, null, \"rex_str\", 2, [] ]\n"
      "  ] ],\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 2, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"cd\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"abc+\" (failure)"
  {
    i.string = "abd";
    t = fabr_rex("x", &i, "abc+");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 4, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 4, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 0, 2, 0, null, \"rex_rep\", 2, [\n"
      "      [ null, 0, 2, 0, null, \"rex_str\", 1, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"abc+\" (success)"
  {
    i.string = "abcc";
    t = fabr_rex("x", &i, "abc+");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex\", 4, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 4, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 2, null, \"rex_rep\", 2, [\n"
      "      [ null, 1, 2, 1, null, \"rex_str\", 1, \"c\" ],\n"
      "      [ null, 1, 3, 1, null, \"rex_str\", 1, \"c\" ],\n"
      "      [ null, 0, 4, 0, null, \"rex_str\", 1, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"abc+\" (success) (prune)"
  {
    i.string = "abcc";
    i.flags = FABR_F_PRUNE;
    t = fabr_rex("x", &i, "abc+");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex\", 4, \"abcc\" ]");
  }

  it "accepts \"ab[cd]\" (failure)"
  {
    i.string = "abe";
    t = fabr_rex("x", &i, "ab[cd]");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 6, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 6, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 0, 2, 0, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]\" (success)"
  {
    i.string = "abc";
    t = fabr_rex("x", &i, "ab[cd]");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex\", 6, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 6, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 1, null, \"rng\", 4, \"c\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]ef\" (success)"
  {
    i.string = "abcef";
    t = fabr_rex("x", &i, "ab[cd]ef");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 5, null, \"rex\", 8, [\n"
      "  [ null, 1, 0, 5, null, \"rex_seq\", 8, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 1, null, \"rng\", 4, \"c\" ],\n"
      "    [ null, 1, 3, 2, null, \"rex_str\", 2, \"ef\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]cd\" (failure)"
  {
    i.string = "ccd";
    t = fabr_rex("x", &i, "[ab]cd");

    expect(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 6, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 6, [\n"
      "    [ null, 0, 0, 0, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]cd\" (success)"
  {
    i.string = "acd";
    t = fabr_rex("x", &i, "[ab]cd");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex\", 6, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 6, [\n"
      "    [ null, 1, 0, 1, null, \"rng\", 4, \"a\" ],\n"
      "    [ null, 1, 1, 2, null, \"rex_str\", 2, \"cd\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]+\" (failure)"
  {
    i.string = "abe";
    t = fabr_rex("x", &i, "ab[cd]+");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 7, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
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

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex\", 7, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 1, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, \"c\" ],\n"
      "      [ null, 0, 3, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]+\" (success 2)"
  {
    i.string = "abcd";
    t = fabr_rex("x", &i, "ab[cd]+");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex\", 7, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 2, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, \"c\" ],\n"
      "      [ null, 1, 3, 1, null, \"rng\", 2, \"d\" ],\n"
      "      [ null, 0, 4, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]*\" (failure)"
  {
    i.string = "abe";
    t = fabr_rex("x", &i, "ab[cd]*");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex\", 7, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
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

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex\", 7, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 0, null, \"rex_rep\", 5, [\n"
      "      [ null, 0, 2, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab[cd]*\" (success 2)"
  {
    i.string = "abcd";
    t = fabr_rex("x", &i, "ab[cd]*");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex\", 7, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 2, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, \"c\" ],\n"
      "      [ null, 1, 3, 1, null, \"rng\", 2, \"d\" ],\n"
      "      [ null, 0, 4, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]\" (failure)"
  {
    i.string = "catapult";
    t = fabr_rex("x", &i, "[ab]");

    expect(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 4, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 4, [\n"
      "    [ null, 0, 0, 0, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]\" (success)"
  {
    i.string = "arles";
    t = fabr_rex("x", &i, "[ab]");

    expect(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 1, null, \"rex\", 4, [\n"
      "  [ null, 1, 0, 1, null, \"rex_seq\", 4, [\n"
      "    [ null, 1, 0, 1, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]+\" (failure)"
  {
    i.string = "carl";
    t = fabr_rex("x", &i, "[ab]+");

    expect(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 5, [\n"
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

    expect(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex\", 5, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 5, [\n"
      "    [ null, 1, 0, 3, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, [] ],\n"
      "      [ null, 1, 1, 1, null, \"rng\", 2, [] ],\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, [] ],\n"
      "      [ null, 0, 3, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]?\" (success)"
  {
    i.string = "c";
    t = fabr_rex("x", &i, "[ab]?");

    expect(fabr_tree_to_string(t, NULL, 0) ===f ""
      "[ \"x\", 1, 0, 0, null, \"rex\", 5, [\n"
      "  [ null, 1, 0, 0, null, \"rex_seq\", 5, [\n"
      "    [ null, 1, 0, 0, null, \"rex_rep\", 5, [\n"
      "      [ null, 0, 0, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]?\" (success 2)"
  {
    i.string = "a";
    t = fabr_rex("x", &i, "[ab]?");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 1, null, \"rex\", 5, [\n"
      "  [ null, 1, 0, 1, null, \"rex_seq\", 5, [\n"
      "    [ null, 1, 0, 1, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, \"a\" ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]*\" (success)"
  {
    i.string = "c";
    t = fabr_rex("x", &i, "[ab]*");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 0, null, \"rex\", 5, [\n"
      "  [ null, 1, 0, 0, null, \"rex_seq\", 5, [\n"
      "    [ null, 1, 0, 0, null, \"rex_rep\", 5, [\n"
      "      [ null, 0, 0, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]*\" (success 2)"
  {
    i.string = "aa";
    t = fabr_rex("x", &i, "[ab]*");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex\", 5, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 5, [\n"
      "    [ null, 1, 0, 2, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 1, 1, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 0, 2, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]{2}\" (failure)"
  {
    i.string = "a";
    t = fabr_rex("x", &i, "[ab]{2}");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 7, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 7, [\n"
      "    [ null, 0, 0, 0, null, \"rex_rep\", 7, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 0, 1, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]{2}\" (success)"
  {
    i.string = "aa";
    t = fabr_rex("x", &i, "[ab]{2}");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex\", 7, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 2, null, \"rex_rep\", 7, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 1, 1, 1, null, \"rng\", 2, \"a\" ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]{2,3}\" (failure)"
  {
    i.string = "a";
    t = fabr_rex("z", &i, "[ab]{2,3}");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 0, 0, 0, null, \"rex\", 9, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 9, [\n"
      "    [ null, 0, 0, 0, null, \"rex_rep\", 9, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 0, 1, 0, null, \"rng\", 2, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]{2,3}\" (success)"
  {
    i.string = "aaa";
    t = fabr_rex("z", &i, "[ab]{2,3}");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 1, 0, 3, null, \"rex\", 9, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 3, null, \"rex_rep\", 9, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 1, 1, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, \"a\" ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab]{2,3}\" (success 2)"
  {
    i.string = "aaaa";
    t = fabr_rex("z", &i, "[ab]{2,3}");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 1, 0, 3, null, \"rex\", 9, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 3, null, \"rex_rep\", 9, [\n"
      "      [ null, 1, 0, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 1, 1, 1, null, \"rng\", 2, \"a\" ],\n"
      "      [ null, 1, 2, 1, null, \"rng\", 2, \"a\" ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab(cd|ef)\" (failure)"
  {
    i.string = "abgh";
    t = fabr_rex("x", &i, "ab(cd|ef)");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 9, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 0, 2, 0, null, \"rex_alt\", 7, [\n"
      "      [ null, 0, 2, 0, null, \"rex_seq\", 2, [\n"
      "        [ null, 0, 2, 0, null, \"rex_str\", 2, [] ]\n"
      "      ] ],\n"
      "      [ null, 0, 2, 0, null, \"rex_seq\", 2, [\n"
      "        [ null, 0, 2, 0, null, \"rex_str\", 2, [] ]\n"
      "      ] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab(cd|ef)\" (success)"
  {
    i.string = "abef";
    t = fabr_rex("x", &i, "ab(cd|ef)");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 4, null, \"rex\", 9, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 2, null, \"rex_alt\", 7, [\n"
      "      [ null, 0, 2, 0, null, \"rex_seq\", 2, [\n"
      "        [ null, 0, 2, 0, null, \"rex_str\", 2, [] ]\n"
      "      ] ],\n"
      "      [ null, 1, 2, 2, null, \"rex_seq\", 2, [\n"
      "        [ null, 1, 2, 2, null, \"rex_str\", 2, \"ef\" ]\n"
      "      ] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab(cd)?ef\" (failure)"
  {
    i.string = "abxef";
    t = fabr_rex("x", &i, "ab(cd)?ef");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 9, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 0, null, \"rex_rep\", 5, [\n"
      "      [ null, 0, 2, 0, null, \"rex_alt\", 2, [\n"
      "        [ null, 0, 2, 0, null, \"rex_seq\", 2, [\n"
      "          [ null, 0, 2, 0, null, \"rex_str\", 2, [] ]\n"
      "        ] ]\n"
      "      ] ]\n"
      "    ] ],\n"
      "    [ null, 0, 2, 0, null, \"rex_str\", 2, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab(cd)?ef\" (success)"
  {
    i.string = "abcdef";
    t = fabr_rex("x", &i, "ab(cd)?ef");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 6, null, \"rex\", 9, [\n"
      "  [ null, 1, 0, 6, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 2, null, \"rex_rep\", 5, [\n"
      "      [ null, 1, 2, 2, null, \"rex_alt\", 2, [\n"
      "        [ null, 1, 2, 2, null, \"rex_seq\", 2, [\n"
      "          [ null, 1, 2, 2, null, \"rex_str\", 2, \"cd\" ]\n"
      "        ] ]\n"
      "      ] ]\n"
      "    ] ],\n"
      "    [ null, 1, 4, 2, null, \"rex_str\", 2, \"ef\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"ab(cd)ef\" (success)"
  {
    i.string = "abcdef";
    t = fabr_rex("x", &i, "ab(cd)ef");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 6, null, \"rex\", 8, [\n"
      "  [ null, 1, 0, 6, null, \"rex_seq\", 8, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 1, 2, 2, null, \"rex_alt\", 4, [\n"
      "      [ null, 1, 2, 2, null, \"rex_seq\", 2, [\n"
      "        [ null, 1, 2, 2, null, \"rex_str\", 2, \"cd\" ]\n"
      "      ] ]\n"
      "    ] ],\n"
      "    [ null, 1, 4, 2, null, \"rex_str\", 2, \"ef\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"(ab)\" (success)"
  {
    i.string = "ab";
    t = fabr_rex("x", &i, "(ab)");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 2, null, \"rex\", 4, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 4, [\n"
      "    [ null, 1, 0, 2, null, \"rex_alt\", 4, [\n"
      "      [ null, 1, 0, 2, null, \"rex_seq\", 2, [\n"
      "        [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ]\n"
      "      ] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab][cd]\" (failure)"
  {
    i.string = "aa";
    t = fabr_rex("y", &i, "[ab][cd]");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"y\", 0, 0, 0, null, \"rex\", 8, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 8, [\n"
      "    [ null, 1, 0, 1, null, \"rng\", 4, \"a\" ],\n"
      "    [ null, 0, 1, 0, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"a.c\" (success)"
  {
    i.string = "abc";
    t = fabr_rex("y", &i, "a.c");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"y\", 1, 0, 3, null, \"rex\", 3, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 3, [\n"
      "    [ null, 1, 0, 3, null, \"rex_str\", 3, \"abc\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"[ab][cd]\" (success)"
  {
    i.string = "ac";
    t = fabr_rex("y", &i, "[ab][cd]");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"y\", 1, 0, 2, null, \"rex\", 8, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 8, [\n"
      "    [ null, 1, 0, 1, null, \"rng\", 4, \"a\" ],\n"
      "    [ null, 1, 1, 1, null, \"rng\", 4, \"c\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"=[^\\r\\n]*\" (success)"
  {
    i.string = "=ab";

    t = fabr_rex("x", &i, "=[^\r\n]*");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 0, 3, null, \"rex\", 7, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 7, [\n"
      "    [ null, 1, 0, 1, null, \"rex_str\", 1, \"=\" ],\n"
      "    [ null, 1, 1, 2, null, \"rex_rep\", 6, [\n"
      "      [ null, 1, 1, 1, null, \"rng\", 3, \"a\" ],\n"
      "      [ null, 1, 2, 1, null, \"rng\", 3, \"b\" ],\n"
      "      [ null, 0, 3, 0, null, \"rng\", 3, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"\\\\|\\\\|?\" (success)"
  {
    i.string = "||";
    t = fabr_rex("p", &i, "\\|\\|?");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"p\", 1, 0, 2, null, \"rex\", 5, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 5, [\n"
      "    [ null, 1, 0, 1, null, \"rex_str\", 3, \"|\" ],\n"
      "    [ null, 1, 1, 1, null, \"rex_rep\", 2, [\n"
      "      [ null, 1, 1, 1, null, \"rex_str\", 1, \"|\" ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts lone \\ escapes"
  {
    i.string = "deux";
    t = fabr_rex("y", &i, "de\\u[x-z]");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"y\", 1, 0, 4, null, \"rex\", 9, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 9, [\n"
      "    [ null, 1, 0, 3, null, \"rex_str\", 4, \"deu\" ],\n"
      "    [ null, 1, 3, 1, null, \"rng\", 5, \"x\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \\\\ escapes"
  {
    i.string = "de\\ux";
    t = fabr_rex("y", &i, "de\\\\u[x-z]");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"y\", 1, 0, 5, null, \"rex\", 10, [\n"
      "  [ null, 1, 0, 5, null, \"rex_seq\", 10, [\n"
      "    [ null, 1, 0, 4, null, \"rex_str\", 5, \"de\\\\u\" ],\n"
      "    [ null, 1, 4, 1, null, \"rng\", 5, \"x\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "resets the input offset in case of failure"
  {
    i.string = "abCef";
    t = fabr_rex("x", &i, "ab[cd]ef");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 0, 0, 0, null, \"rex\", 8, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 8, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 2, \"ab\" ],\n"
      "    [ null, 0, 2, 0, null, \"rng\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");

    expect(i.offset zu== 0);
  }

  it "accepts an empty input"
  {
    i.string = "abc";
    i.offset = 3;

    t = fabr_rex("z", &i, "[a-z]+");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"z\", 0, 3, 0, null, \"rex\", 6, [\n"
      "  [ null, 0, 3, 0, null, \"rex_seq\", 6, [\n"
      "    [ null, 0, 3, 0, null, \"rex_rep\", 6, [\n"
      "      [ null, 0, 3, 0, null, \"rng\", 3, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "matches an empty input on \"a*\""
  {
    i.string = "ab";
    i.offset = 2;

    t = fabr_rex("x", &i, "a*");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"x\", 1, 2, 0, null, \"rex\", 2, [\n"
      "  [ null, 1, 2, 0, null, \"rex_seq\", 2, [\n"
      "    [ null, 1, 2, 0, null, \"rex_rep\", 2, [\n"
      "      [ null, 0, 2, 0, null, \"rex_str\", 1, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "matches an empty input on \"a?\""
  {
    i.string = "ab";
    i.offset = 2;

    t = fabr_rex("y", &i, "a?");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"y\", 1, 2, 0, null, \"rex\", 2, [\n"
      "  [ null, 1, 2, 0, null, \"rex_seq\", 2, [\n"
      "    [ null, 1, 2, 0, null, \"rex_rep\", 2, [\n"
      "      [ null, 0, 2, 0, null, \"rex_str\", 1, [] ]\n"
      "    ] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "matches the eol $ (failure)"
  {
    i.string = "abcd";

    t = fabr_rex("nl", &i, "ab$");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"nl\", 0, 0, 0, null, \"rex\", 3, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 3, [\n"
      "    [ null, 0, 0, 0, null, \"rex_str\", 3, [] ]\n"
      "  ] ]\n"
      "] ]");

    expect(i.offset zu== 0);
  }

  it "matches the eol $ (success)"
  {
    i.string = "ab\ncd";

    t = fabr_rex("nl", &i, "ab$");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"nl\", 1, 0, 2, null, \"rex\", 3, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 3, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 3, \"ab\" ]\n"
      "  ] ]\n"
      "] ]");

    expect(i.offset zu== 2);
  }

  it "matches the eos $ (failure)"
  {
    i.string = "abc";

    t = fabr_rex("0", &i, "ab$");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"0\", 0, 0, 0, null, \"rex\", 3, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 3, [\n"
      "    [ null, 0, 0, 0, null, \"rex_str\", 3, [] ]\n"
      "  ] ]\n"
      "] ]");

    expect(i.offset zu== 0);
  }

  it "matches the eos $ (success)"
  {
    i.string = "ab";

    t = fabr_rex("0", &i, "ab$");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"0\", 1, 0, 2, null, \"rex\", 3, [\n"
      "  [ null, 1, 0, 2, null, \"rex_seq\", 3, [\n"
      "    [ null, 1, 0, 2, null, \"rex_str\", 3, \"ab\" ]\n"
      "  ] ]\n"
      "] ]");

    expect(i.offset zu== 2);
  }

  it "accepts an escaped $ (failure)"
  {
    i.string = "abDcd";

    t = fabr_rex("d", &i, "ab\\$");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"d\", 0, 0, 0, null, \"rex\", 4, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 4, [\n"
      "    [ null, 0, 0, 0, null, \"rex_str\", 4, [] ]\n"
      "  ] ]\n"
      "] ]");

    expect(i.offset zu== 0);
  }

  it "accepts an escaped $ (success)"
  {
    i.string = "ab$cd";

    t = fabr_rex("d", &i, "ab\\$");

    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"d\", 1, 0, 3, null, \"rex\", 4, [\n"
      "  [ null, 1, 0, 3, null, \"rex_seq\", 4, [\n"
      "    [ null, 1, 0, 3, null, \"rex_str\", 4, \"ab$\" ]\n"
      "  ] ]\n"
      "] ]");

    expect(i.offset zu== 3);
  }

  it "doesn't go into an infinite loop for 'djan blanks'"
  {
    i.string = "whatever";

    //([ \t]*
    //  (
    //    (#[^\r\n]*)?
    //    ([\r\n]|$)
    //  )?
    //)*

    t = fabr_rex("e", &i, "([ \t]*((#[^\r\n]*)?([\r\n]|$))?)*");

    expect(t->result i== 1);
    expect(t->length zu== 0);

    expect(i.offset zu== 0);
  }

  it "accepts \"over\\Z\" (end of string) (failure)"
  {
    i.string = "over\nzealous";
    t = fabr_rex("Z", &i, "over\\Z");

    //fabr_puts(t, i.string, 3);
    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"Z\", 0, 0, 0, null, \"rex\", 6, [\n"
      "  [ null, 0, 0, 0, null, \"rex_seq\", 6, [\n"
      "    [ null, 0, 0, 0, null, \"rex_str\", 6, [] ]\n"
      "  ] ]\n"
      "] ]");
  }

  it "accepts \"over\\Z\" (end of string) (success)"
  {
    i.string = "over";
    t = fabr_rex("Z", &i, "over\\Z");

    //fabr_puts(t, i.string, 3);
    expect(fabr_tree_to_string(t, i.string, 0) ===f ""
      "[ \"Z\", 1, 0, 4, null, \"rex\", 6, [\n"
      "  [ null, 1, 0, 4, null, \"rex_seq\", 6, [\n"
      "    [ null, 1, 0, 4, null, \"rex_str\", 6, \"over\" ]\n"
      "  ] ]\n"
      "] ]");
  }

  context "errors"
  {
    it "rejects \"[ab\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "[ab");

      expect(fabr_tree_to_string(t, NULL, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex\", 3, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 3, [\n"
        "    [ null, -1, 0, 0, \"range not closed >[ab<3\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"a{1,2fda\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "a{1,2fda");

      expect(fabr_tree_to_string(t, NULL, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex\", 8, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 8, [\n"
        "    [ null, -1, 0, 0, \"invalid {min[,max]} >a{1,2fda<8\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"a{1,2fda}nada\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "a{1,2fda}nada");

      expect(fabr_tree_to_string(t, NULL, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex\", 13, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 13, [\n"
        "    [ null, -1, 0, 0, \"invalid {min[,max]} >a{1,2fda}nada<13\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"a{1,2,3}nada\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "a{1,2,3}nada");

      expect(fabr_tree_to_string(t, NULL, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex\", 12, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 12, [\n"
        "    [ null, -1, 0, 0, \"invalid {min[,max]} >a{1,2,3}nada<12\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"1**\""
    {
      i.string = "nada";
      t = fabr_rex("x", &i, "1**");

      expect(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"x\", -1, 0, 0, null, \"rex\", 3, [\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 3, [\n"
        "    [ null, 1, 0, 0, null, \"rex_rep\", 2, [\n"
        "      [ null, 0, 0, 0, null, \"rex_str\", 1, [] ]\n"
        "    ] ],\n"
        "    [ null, -1, 0, 0, \"lone quantifier >*<1\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"ab|cd**\""
    {
      i.string = "ca";
      t = fabr_rex("y", &i, "ab|cd**");

      expect(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"y\", -1, 0, 0, null, \"rex\", 7, [\n"
        "  [ null, 0, 0, 0, null, \"rex_seq\", 2, [\n"
        "    [ null, 0, 0, 0, null, \"rex_str\", 2, [] ]\n"
        "  ] ],\n"
        "  [ null, -1, 0, 0, null, \"rex_seq\", 4, [\n"
        "    [ null, 1, 0, 1, null, \"rex_str\", 1, \"c\" ],\n"
        "    [ null, 1, 1, 0, null, \"rex_rep\", 2, [\n"
        "      [ null, 0, 1, 0, null, \"rex_str\", 1, [] ]\n"
        "    ] ],\n"
        "    [ null, -1, 1, 0, \"lone quantifier >*<1\", \"rex_rep\", 0, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "does not reject \"ab|cd**\" (not reaching *)"
    {
      i.string = "nada";
      t = fabr_rex("y", &i, "ab|cd**");

      expect(fabr_tree_to_string(t, i.string, 0) ===f ""
        "[ \"y\", 0, 0, 0, null, \"rex\", 7, [\n"
        "  [ null, 0, 0, 0, null, \"rex_seq\", 2, [\n"
        "    [ null, 0, 0, 0, null, \"rex_str\", 2, [] ]\n"
        "  ] ],\n"
        "  [ null, 0, 0, 0, null, \"rex_seq\", 4, [\n"
        "    [ null, 0, 0, 0, null, \"rex_str\", 1, [] ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "rejects \"ab**|cd\""
    {
      i.string = "acadabra";
      t = fabr_rex("x", &i, "ab**|cd");

      //fabr_puts(t, i.string, 3);
      expect(t->result d== -1);
    }

    it "rejects \"(ab**|cd)\""
    {
      i.string = "acadabra";
      t = fabr_rex("x", &i, "(ab**|cd)");

      //fabr_puts(t, i.string, 3);
      expect(t->result d== -1);
    }
  }
}

