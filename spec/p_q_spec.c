
//
// specifying aabro
//
// Mon Sep 15 07:36:48 JST 2014
//

#include "aabro.h"


describe "fabr_q()"
{
  before each
  {
    fabr_tree *t = NULL;
    fabr_parser *p = NULL;
  }
  after each
  {
    if (t != NULL) fabr_tree_free(t);
    if (p != NULL) fabr_parser_free(p);
  }

  describe "fabr_q()"
  {
    it "returns a 'marker' parser"
    {
      p = fabr_q("{4,5}");

      ensure(p->type == 10);
      ensure(p->string === "{4,5}");
    }

    it "wraps the preceding parser"
    {
      p = fabr_seq(fabr_string("x"), fabr_q("+"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_seq( /* 0 */\n"
        "  fabr_rep( /* 00 */\n"
        "    fabr_string(\"x\") /* 000 */, 1, -1),\n"
        "  NULL)");
    }

    it "returns an error marker if there are not preceding parser to wrap"
    {
      p = fabr_seq(fabr_q("+"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_seq( /* 0 */\n"
        "  fabr_error(\"'+': no preceding parser to wrap\") /* 00 */,\n"
        "  NULL)");
    }

    it "does not steal the name of the wrapped parser"
    {
      p = fabr_seq(fabr_n_string("sn", "x"), fabr_q("+"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_seq( /* 0 */\n"
        "  fabr_rep( /* 00 */\n"
        "    fabr_n_string(\"sn\", \"x\") /* 000 */, 1, -1),\n"
        "  NULL)");
    }
  }

  describe "fabr_n_q()"
  {
    it "keeps its own name"
    {
      p = fabr_seq(fabr_n_string("sn", "x"), fabr_n_q("qn", "+"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_seq( /* 0 */\n"
        "  fabr_n_rep( /* 00 */\n"
        "    \"qn\",\n"
        "    fabr_n_string(\"sn\", \"x\") /* 000 */, 1, -1),\n"
        "  NULL)");
    }

    it "keeps its own name (2)"
    {
      p = fabr_seq(fabr_n_string("sn", "x"), fabr_n_q("", "+"), NULL);
      char *s = fabr_parser_to_string(p);

      ensure(s ===f ""
        "fabr_seq( /* 0 */\n"
        "  fabr_n_rep( /* 00 */\n"
        "    \"\",\n"
        "    fabr_n_string(\"sn\", \"x\") /* 000 */, 1, -1),\n"
        "  NULL)");
    }
  }
}

