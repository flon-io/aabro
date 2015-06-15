
//
// specifying aabro
//
// Mon Jun  8 07:45:29 JST 2015
//
// Ma jonque est jaune
//

#include "aabro.h"


context "combinations"
{
  before each
  {
    fabr_tree *t = NULL;
  }
  after each
  {
    fabr_tree_free(t);
  }

  static fabr_tree *_cbo_text_line(fabr_input *i)
  {
    return fabr_rex("tl", i, "[a-z]+");
  }
  static fabr_tree *_cbo_blank_line(fabr_input *i)
  {
    return fabr_rex("bl", i, "[ \t]*");
  }
  static fabr_tree *_cbo_line(fabr_input *i)
  {
    return fabr_alt("l", i, _cbo_text_line, _cbo_blank_line, NULL);
  }

  static fabr_tree *_cbo_lines_star(fabr_input *i)
  {
    return fabr_seq("ls", i, _cbo_line, fabr_star, NULL);
  }

  describe "fab_star when no progress"
  {
    it "stops"
    {
      char *s = "abc";

      //t = fabr_parse_f(s, _cbo_lines_star, 0);
      //fabr_puts_tree(t, s, 1);

      t = fabr_parse_all(s, _cbo_lines_star);

      expect(fabr_tree_to_string(t, s, 0) ===f ""
        "[ null, 1, 0, 3, null, \"all\", 0, [\n"
        "  [ \"ls\", 1, 0, 3, null, \"seq\", 0, [\n"
        "    [ \"l\", 1, 0, 3, null, \"alt\", 0, [\n"
        "      [ \"tl\", 1, 0, 3, null, \"rex\", 6, \"abc\" ]\n"
        "    ] ],\n"
        "    [ \"l\", 1, 3, 0, null, \"alt\", 0, [\n"
        "      [ \"bl\", 1, 3, 0, null, \"rex\", 5, \"\" ]\n"
        "    ] ]\n"
        "  ] ]\n"
        "] ]");
    }
  }

  static fabr_tree *_cbo_lines_plus(fabr_input *i)
  {
    return fabr_seq("ls", i, _cbo_line, fabr_plus, NULL);
  }

  describe "fab_plus when no progress"
  {
    it "stops"
    {
      char *s = "abc";

      //t = fabr_parse_f(s, _cbo_lines_plus, 0);
      //fabr_puts_tree(t, s, 1);

      t = fabr_parse_all(s, _cbo_lines_plus);

      expect(fabr_tree_to_string(t, s, 0) ===f ""
        "[ null, 1, 0, 3, null, \"all\", 0, [\n"
        "  [ \"ls\", 1, 0, 3, null, \"seq\", 0, [\n"
        "    [ \"l\", 1, 0, 3, null, \"alt\", 0, [\n"
        "      [ \"tl\", 1, 0, 3, null, \"rex\", 6, \"abc\" ]\n"
        "    ] ],\n"
        "    [ \"l\", 1, 3, 0, null, \"alt\", 0, [\n"
        "      [ \"bl\", 1, 3, 0, null, \"rex\", 5, \"\" ]\n"
        "    ] ]\n"
        "  ] ]\n"
        "] ]");
    }
  }

  static fabr_tree *_cbo_lines_rep(fabr_input *i)
  {
    return fabr_rep("ls", i, _cbo_line, 0, 0);
  }

  describe "fab_rep when no progress"
  {
    it "stops"
    {
      char *s = "abc";

      //t = fabr_parse_f(s, _cbo_lines_rep, 0);
      //fabr_puts_tree(t, s, 1);

      t = fabr_parse_all(s, _cbo_lines_rep);

      expect(fabr_tree_to_string(t, s, 0) ===f ""
        "[ null, 1, 0, 3, null, \"all\", 0, [\n"
        "  [ \"ls\", 1, 0, 3, null, \"rep\", 0, [\n"
        "    [ \"l\", 1, 0, 3, null, \"alt\", 0, [\n"
        "      [ \"tl\", 1, 0, 3, null, \"rex\", 6, \"abc\" ]\n"
        "    ] ],\n"
        "    [ \"l\", 1, 3, 0, null, \"alt\", 0, [\n"
        "      [ \"bl\", 1, 3, 0, null, \"rex\", 5, \"\" ]\n"
        "    ] ]\n"
        "  ] ]\n"
        "] ]");
    }
  }
}

