
//
// specifying aabro
//
// Mon Apr 21 06:06:30 JST 2014
//

#include "aabro.h"

context "sample, arith"
{
  before all
  {
    //abr_parser *number =
    //  abr_regex_s("^-?[0-9]+");
    //abr_parser *parentheses =
    //  abr_seq(abr_string("("), expression, abr_string(")"));
    //abr_parser *value =
    //  abr_alt(parentheses, number);
    //abr_parser *operator =
    //  abr_regex_s("^[\+\-\*\/]");
    //abr_parser *operation =
    //  abr_seq(value, abr_rep(abr_seq(operator, value), 0, -1));
    //
    //abr_parser *expression =
    //  operation;
  }

  it "parses numbers"
  {
    ensure(1 == 1);
  }
}

