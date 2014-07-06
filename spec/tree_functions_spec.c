
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

    abr_parser *string =
      abr_n_regex(
        "string",
        "^\"("
          //"\\\\." "|"
          "\\\\[\"\\/\\\\bfnrt]" "|"
          "\\\\u[0-9a-fA-F]{4}" "|"
          "[^\"\\]"
        ")*\"");

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

    abr_parser *number =
      abr_n_regex("number", "^-?[0-9]+(\\.[0-9]+)?");

    abr_parser *p=
      abr_n_alt(
        "value",
        number,
        array,
        string,
        NULL);
  }
  after each
  {
    if (t != NULL) abr_tree_free(t);
    if (p != NULL) abr_parser_free(p);
  }

  //typedef int abr_tree_func(abr_tree *);
  //
  short is_value(const abr_tree *t)
  {
    // -1: fail, do not continue
    //  0: fail, please check my children
    //  1: success

    if (t->result != 1) return -1;
    return t->name && strcmp(t->name, "value") == 0;
  }

  describe "abr_tree_list()"
  {
    it "collects the trees that match the given function"
    {
      char *s = "[1,2,3]";
      t = abr_parse_all(s, 0, p);
      //char *st = abr_tree_to_string(t); puts(st); free(st);

      flu_list *l = abr_tree_list(abr_t_child(t, 0), is_value);

      ensure(l->size == 3);

      abr_tree *t0 = (abr_tree *)flu_list_at(l, 0);
      abr_tree *t1 = (abr_tree *)flu_list_at(l, 1);
      abr_tree *t2 = (abr_tree *)flu_list_at(l, 2);

      ensure(t0->name === "value");
      ensure(abr_tree_string(s, t0) ===f "1");
      ensure(t1->name === "value");
      ensure(abr_tree_string(s, t1) ===f "2");
      ensure(t2->name === "value");
      ensure(abr_tree_string(s, t2) ===f "3");

      ensure(abr_tree_to_string(t0) ===f ""
        "[ \"value\", 1, 1, 1, null, \"alt\", [\n"
        "  [ \"number\", 1, 1, 1, null, \"regex\", [] ]\n"
        "] ]"
      );

      flu_list_free(l);
        // NB: only freeing this list of pointers,
        //     the trees themselves are freed in the "after each"
    }
  }

  describe "abr_tree_list_named()"
  {
    it "collects the trees that match the given name"
    {
      char *s = "[1,2,3]";
      t = abr_parse_all(s, 0, p);
      //char *st = abr_tree_to_string(t); puts(st); free(st);

      flu_list *l = abr_tree_list_named(abr_t_child(t, 0), "value");

      ensure(l->size == 3);

      abr_tree *t0 = (abr_tree *)flu_list_at(l, 0);
      abr_tree *t1 = (abr_tree *)flu_list_at(l, 1);
      abr_tree *t2 = (abr_tree *)flu_list_at(l, 2);

      ensure(t0->name === "value");
      ensure(abr_tree_string(s, t0) ===f "1");
      ensure(t1->name === "value");
      ensure(abr_tree_string(s, t1) ===f "2");
      ensure(t2->name === "value");
      ensure(abr_tree_string(s, t2) ===f "3");

      flu_list_free(l);
        // NB: only freeing this list of pointers,
        //     the trees themselves are freed in the "after each"
    }
  }

  describe "abr_tree_collect()"
  {
    it "collects the trees that match the given function"
    {
      char *s = "[1,2,3]";
      t = abr_parse_all(s, 0, p);
      //char *st = abr_tree_to_string(t); puts(st); free(st);

      abr_tree **ts = abr_tree_collect(t->child, is_value);

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
        "          ] ]\n"
        "        ] ]\n"
        "      ] ]\n"
        "    ] ],\n"
        "    [ null, 1, 7, 1, null, \"string\", \"]\" ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "escapes the leave string"
    {
      char *in = "\"hello\nworld\"";
      t = abr_parse_all(in, 0, p);
      char *s = abr_tree_to_string_with_leaves(in, t);

      ensure(s ===f ""
        "[ \"value\", 1, 0, 13, null, \"alt\", [\n"
        "  [ \"string\", 1, 0, 13, null, \"regex\", \"\\\"hello\\nworld\\\"\" ]\n"
        "] ]");
    }
  }

  describe "abr_tree_lookup()"
  {
    it "returns NULL if it finds nothing"
    {
      char *s = "[1,2,3]";
      t = abr_parse_all(s, 0, p);

      //puts(abr_tree_to_string_with_leaves(s, t));

      abr_tree *r = abr_tree_lookup(t, "string");

      ensure(r == NULL);
    }

    it "returns the first sub-tree with the given name"
    {
      char *s = "[1,\"deux\",3]";
      t = abr_parse_all(s, 0, p);

      //puts(abr_tree_to_string_with_leaves(s, t));

      abr_tree *r = abr_tree_lookup(t, "string");

      ensure(r != NULL);
      ensure(r->name === "string");
      ensure(r->offset == 3);
      ensure(r->length == 6);
    }
  }
}

