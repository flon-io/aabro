
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
      char *s = "[1,2,3]";
      t = abr_parse_all(s, 0, p);
      //char *st = abr_tree_to_string(t); puts(st); free(st);

      abr_tree **ts = abr_tree_collect(t->children[1], is_value);

      ensure(ts[0] != NULL);
      ensure(ts[1] != NULL);
      ensure(ts[2] != NULL);
      ensure(ts[3] == NULL);

      ensure(ts[0]->name === "value");
      ensure(abr_tree_string(s, ts[0]) ===f "1");
      ensure(ts[1]->name === "value");
      ensure(abr_tree_string(s, ts[1]) ===f "2");
      ensure(ts[2]->name === "value");
      ensure(abr_tree_string(s, ts[2]) ===f "3");

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

  describe "abr_tree_to_string()"
  {
    it "returns a string representation of an abr_tree"
    {
      t = abr_parse_all("-1", 0, p);
      char *s = abr_tree_to_string(t);

      //puts(s);
      ensure(s ===f ""
        "[ \"value\", 1, 0, 2, null, \"alt\", [\n"
        "  [ \"number\", 1, 0, 2, null, \"regex\", [] ]\n"
        "] ]");
    }
  }

  describe "abr_tree_to_string_with_leaves()"
  {
    it "returns a string representation of an abr_tree with string leaves"
    {
      char *in = "[-1,0,1]";
      t = abr_parse_all(in, 0, p);
      char *s = abr_tree_to_string_with_leaves(in, t);

      //puts(s);
      ensure(s ===f ""
        "[ \"value\", 1, 0, 8, null, \"alt\", [\n"
        "  [ \"number\", 0, 0, 0, null, \"regex\", [] ],\n"
        "  [ \"array\", 1, 0, 8, null, \"seq\", [\n"
        "    [ null, 1, 0, 1, null, \"string\", \"[\" ],\n"
        "    [ \"values\", 1, 1, 6, null, \"rep\", [\n"
        "      [ null, 1, 1, 6, null, \"seq\", [\n"
        "        [ \"value\", 1, 1, 2, null, \"alt\", [\n"
        "          [ \"number\", 1, 1, 2, null, \"regex\", \"-1\" ]\n"
        "        ] ],\n"
        "        [ null, 1, 3, 4, null, \"rep\", [\n"
        "          [ null, 1, 3, 2, null, \"seq\", [\n"
        "            [ null, 1, 3, 1, null, \"string\", \",\" ],\n"
        "            [ \"value\", 1, 4, 1, null, \"alt\", [\n"
        "              [ \"number\", 1, 4, 1, null, \"regex\", \"0\" ]\n"
        "            ] ]\n"
        "          ] ],\n"
        "          [ null, 1, 5, 2, null, \"seq\", [\n"
        "            [ null, 1, 5, 1, null, \"string\", \",\" ],\n"
        "            [ \"value\", 1, 6, 1, null, \"alt\", [\n"
        "              [ \"number\", 1, 6, 1, null, \"regex\", \"1\" ]\n"
        "            ] ]\n"
        "          ] ],\n"
        "          [ null, 0, 7, 0, null, \"seq\", [\n"
        "            [ null, 0, 7, 0, null, \"string\", [] ]\n"
        "          ] ]\n"
        "        ] ]\n"
        "      ] ]\n"
        "    ] ],\n"
        "    [ null, 1, 7, 1, null, \"string\", \"]\" ]\n"
        "  ] ]\n"
        "] ]");
    }
  }
}

