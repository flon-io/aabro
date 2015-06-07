
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

//  static fabr_tree *_cbo_empty(fabr_input *i)
//  {
//    return fabr_rex("e", i, "[ \t]*");
//  }
//  static fabr_tree *_cbo_empties(fabr_input *i)
//  {
//    return fabr_rep("es", i, _cbo_empty, 1, 0);
//  }

  describe "empty string vs 0 or more"
  {
    it "suceeds"
//    {
//      char *s = "";
//
//      t = fabr_parse_all(s, _cbo_empties);
//      fabr_puts_tree(t, s, 1);
//
//      //expect(fabr_tree_to_string(t, s, 0) ===f ""
//      //  "[ null, 1, 0, 6, null, \"all\", 0, [\n"
//      //  "  [ \"value\", 1, 0, 6, null, \"alt\", 0, [\n"
//      //  "    [ \"string\", 1, 0, 6, null, \"rex\", 43, \"\\\"deux\\\"\" ]\n"
//      //  "  ] ]\n"
//      //  "] ]");
//    }
  }
}

