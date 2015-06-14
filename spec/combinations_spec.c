
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
      t = fabr_parse_f(s, _cbo_lines, 0);
      fabr_puts_tree(t, s, 1);
    }
//    //expect(fabr_tree_to_string(t, s, 0) ===f ""
//    //  "[ null, 1, 0, 6, null, \"all\", 0, [\n"
//    //  "  [ \"value\", 1, 0, 6, null, \"alt\", 0, [\n"
//    //  "    [ \"string\", 1, 0, 6, null, \"rex\", 43, \"\\\"deux\\\"\" ]\n"
//    //  "  ] ]\n"
//    //  "] ]");
  }
}

