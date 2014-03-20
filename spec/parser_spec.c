
//
// specifying aabro
//
// Thu Mar 20 06:29:52 JST 2014
//

#include "aabro.h"


describe "abr_parser"
{
  before each
  {
    abr_parser *p = NULL;
  }
  after each
  {
    if (p != NULL) abr_parser_free(p);
  }

  describe "abr_string(s)"
  {
    it "creates a string parser struct"
    {
      p = abr_string("beowulf");

      ensure(p != NULL);
      ensure(p->string === "beowulf");
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_string("toto");

      ensure(abr_parser_to_string(p) ===f "abr_string(\"toto\")");
    }
  }
}

