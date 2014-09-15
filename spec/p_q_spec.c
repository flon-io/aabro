
//
// specifying aabro
//
// Mon Sep 15 07:36:48 JST 2014
//

#include "aabro.h"


describe "abr_q()"
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

  describe "abr_q()"
  {
    it "returns a 'marker' parser"
    {
      p = abr_q("{4,5}");

      ensure(p->type == 11);
      ensure(p->string === "{4,5}");
    }

    it "wraps the preceding parser"
    {
      p = abr_seq(abr_string("x"), abr_q("+"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_rep( /* 00 */\n"
        "    abr_string(\"x\") /* 000 */, 1, -1),\n"
        "  NULL)");
    }

    it "returns an error marker if there are not preceding parser to wrap"
    {
      p = abr_seq(abr_q("+"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_error(\"'+': no preceding parser to wrap\") /* 00 */,\n"
        "  NULL)");
    }

    it "preserves the name of the wrapped parser"
    {
      p = abr_seq(abr_n_string("sn", "x"), abr_n_q("qn", "+"), NULL);
      char *s = abr_parser_to_string(p);

      ensure(s ===f ""
        "abr_seq( /* 0 */\n"
        "  abr_n_rep( /* 00 */\n"
        "    \"qn\",\n"
        "    abr_n_string(\"sn\", \"x\") /* 000 */, 1, -1),\n"
        "  NULL)");
    }
  }

  //describe "abr_n_q()"
  //{
  //}
}

