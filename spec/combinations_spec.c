
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
  static fabr_tree *_cbo_lines(fabr_input *i)
  {
    return fabr_seq("ls", i, _cbo_line, fabr_star, NULL);
  }

  describe "empty string vs 0 or more"
  {
    it "succeeds"
    {
      char *s = "abc";

      //t = fabr_parse_f(s, _cbo_lines, 0);
      //fabr_puts_tree(t, s, 1);

      t = fabr_parse_all(s, _cbo_lines);

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

  // TODO the spec above loops thanks to fabr_star
  //      do try with a fabr_plus() !!!

  // TODO the spec above loops thanks to fabr_star
  //      do try with a fabr_rep() !!!
}

