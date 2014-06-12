
//
// specifying aabro
//
// Wed Jun 11 08:33:21 JST 2014
//

#include "aabro.h"


context "tree functions"
{
  before each
  {
    abr_tree *t = NULL;

    abr_parser *values =
      abr_n_rep(
        "values",
        abr_seq(
          abr_n("value"),
          abr_rep(
            abr_seq(abr_string(","), abr_n("value"), NULL),
            0, -1),
          NULL
        ),
        0, 1);

    abr_parser *array =
      abr_n_seq("array", abr_string("["), values, abr_string("]"), NULL);

    abr_parser *p=
      abr_n_alt(
        "value",
        abr_n_regex("number", "^-?[0-9]+(\\.[0-9]+)?"),
        array,
        NULL);
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
    if (p != NULL) abr_parser_free(p);
  }

  //typedef int abr_tree_func(abr_tree *);
  //
  int is_value(abr_tree *t)
  {
    // -1: fail, do not continue
    //  0: fail, please check my children
    //  1: success

    if (t->result != 1) return -1;
    return t->name && strcmp(t->name, "value") == 0;
  }

  describe "abr_tree_collect()"
  {
    it "works"
    {
      char *in = "[1,2,3]";
      t = abr_parse_all(in, 0, p);
      //char *s = abr_tree_to_string(t); puts(s); free(s);

      abr_tree **ts = abr_tree_collect(t->children[1], is_value);

      ensure(ts[0] != NULL);
      ensure(ts[1] != NULL);
      ensure(ts[2] != NULL);
      ensure(ts[3] == NULL);

      ensure(ts[0]->name === "value");
      ensure(abr_tree_string(in, ts[0]) ===f "1");
      ensure(ts[1]->name === "value");
      ensure(abr_tree_string(in, ts[1]) ===f "2");
      ensure(ts[2]->name === "value");
      ensure(abr_tree_string(in, ts[2]) ===f "3");

      ensure(abr_tree_to_string(ts[0]) ===f ""
        "[ \"value\", 1, 1, 1, null, \"alt\", [\n"
        "  [ \"number\", 1, 1, 1, null, \"regex\", [] ]\n"
        "] ]"
      );

      free(ts);
        // NB: only freeing this list of pointers,
        //     the trees themselves are freed in the "after each"
    }
  }
}

