
//
// specifying aabro
//
// Thu Apr 17 06:02:40 JST 2014
//

#include "aabro.h"


context "regular expressions"
{
  before each
  {
    abr_tree *t = NULL;
    abr_parser *p = NULL;
    regex_t *r = NULL;
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
    if (p != NULL) abr_parser_free(p);
    if (r != NULL) { regfree(r); free(r); }
  }

  describe "abr_regex(s)"
  {
    it "creates a regex parser struct"
    {
      p = abr_regex("a+");

      ensure(p != NULL);
      ensure(p->regex != NULL);
      ensure(p->string === "a+");
      ensure(p->string_length == -1);
    }
  }

  describe "abr_regex_r(r)"
  {
    it "creates a regex parser struct with a borrowed regex"
    {
      r = malloc(sizeof(regex_t));
      regcomp(r, "^a+", REG_EXTENDED);

      p = abr_regex_r(r);

      ensure(p != NULL);
      ensure(p->regex == r);
      ensure(p->string == NULL);
      ensure(p->string_length == -1);
    }
  }

  describe "abr_parser_to_string(p)"
  {
    it "returns a string representation of the parser struct"
    {
      p = abr_regex("^a+");
      char *s = abr_parser_to_string(p);

      ensure(s ===f "abr_regex(\"^a+\")");
    }
    //it "returns a string representation of the parser struct (_r)"
    //{
    //  r = malloc(sizeof(regex_t));
    //  regcomp(r, "^a+", REG_EXTENDED);
    //  p = abr_regex_r(r);
    //  char *s = abr_parser_to_string(p);
    //  ensure(s ===f "abr_regex_r(\"^a+\")");
    //}
  }

  context "parsing"
  {
    it "succeeds"
    {
      p = abr_regex("^x+");
      t = abr_parse("xxx", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f "[ null, 1, 0, 3, \"regex\", [] ]");
    }

    it "fails"
    {
      p = abr_regex("^x+");
      t = abr_parse("yyy", 0, p);
      char *s = abr_tree_to_string(t);

      ensure(s ===f "[ null, 0, 0, -1, \"regex\", [] ]");
    }
  }
}

