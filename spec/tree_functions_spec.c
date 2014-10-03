
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
    fabr_tree *t = NULL;

    fabr_parser *string =
      fabr_n_rex(
        "string",
        "\"("
          //"\\\\." "|"
          "\\\\[\"\\/\\\\bfnrt]" "|"
          "\\\\u[0-9a-fA-F]{4}" "|"
          "[^\"\\\\]"
        ")*\"");
    //fabr_parser *string =
    //  fabr_n_regex(
    //    "string",
    //    "^\"("
    //      //"\\\\." "|"
    //      "\\\\[\"\\/\\\\bfnrt]" "|"
    //      "\\\\u[0-9a-fA-F]{4}" "|"
    //      "[^\"\\]"
    //    ")*\"");

    fabr_parser *values =
      fabr_n_rep(
        "values",
        fabr_seq(
          fabr_n("value"),
          fabr_rep(
            fabr_seq(fabr_string(","), fabr_n("value"), NULL),
            0, -1),
          NULL
        ),
        0, 1);

    fabr_parser *array =
      fabr_n_seq("array", fabr_string("["), values, fabr_string("]"), NULL);

    fabr_parser *number =
      fabr_n_rex("number", "-?[0-9]+(\\.[0-9]+)?");

    fabr_parser *p=
      fabr_n_alt(
        "value",
        number,
        array,
        string,
        NULL);
  }
  after each
  {
    if (t != NULL) fabr_tree_free(t);
    if (p != NULL) fabr_parser_free(p);
  }

  //typedef int fabr_tree_func(fabr_tree *);
  //
  short is_value(const fabr_tree *t)
  {
    // -1: fail, do not continue
    //  0: fail, please check my children
    //  1: success

    if (t->result != 1) return -1;
    return t->name && strcmp(t->name, "value") == 0;
  }

  describe "fabr_tree_list()"
  {
    it "collects the trees that match the given function"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, 0, p);
      //char *st = fabr_tree_to_string(t, NULL); puts(st); free(st);

      flu_list *l = fabr_tree_list(fabr_t_child(t, 0), is_value);

      ensure(l->size == 3);

      fabr_tree *t0 = (fabr_tree *)flu_list_at(l, 0);
      fabr_tree *t1 = (fabr_tree *)flu_list_at(l, 1);
      fabr_tree *t2 = (fabr_tree *)flu_list_at(l, 2);

      ensure(t0->name === "value");
      ensure(fabr_tree_string(s, t0) ===f "1");
      ensure(t1->name === "value");
      ensure(fabr_tree_string(s, t1) ===f "2");
      ensure(t2->name === "value");
      ensure(fabr_tree_string(s, t2) ===f "3");

      ensure(fabr_tree_to_string(t0, NULL) ===f ""
        "[ \"value\", 1, 1, 1, null, \"alt-0\", [\n"
        "  [ \"number\", 1, 1, 1, null, \"rex-00\", [] ]\n"
        "] ]"
      );

      flu_list_free(l);
        // NB: only freeing this list of pointers,
        //     the trees themselves are freed in the "after each"
    }
  }

  describe "fabr_tree_list_named()"
  {
    it "collects the trees that match the given name"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, 0, p);
      //char *st = fabr_tree_to_string(t, NULL); puts(st); free(st);

      flu_list *l = fabr_tree_list_named(fabr_t_child(t, 0), "value");

      ensure(l->size == 3);

      fabr_tree *t0 = (fabr_tree *)flu_list_at(l, 0);
      fabr_tree *t1 = (fabr_tree *)flu_list_at(l, 1);
      fabr_tree *t2 = (fabr_tree *)flu_list_at(l, 2);

      ensure(t0->name === "value");
      ensure(fabr_tree_string(s, t0) ===f "1");
      ensure(t1->name === "value");
      ensure(fabr_tree_string(s, t1) ===f "2");
      ensure(t2->name === "value");
      ensure(fabr_tree_string(s, t2) ===f "3");

      flu_list_free(l);
        // NB: only freeing this list of pointers,
        //     the trees themselves are freed in the "after each"
    }

    it "returns an empty list when it finds nothing"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, 0, p);
      //char *st = fabr_tree_to_string(t, NULL); puts(st); free(st);

      flu_list *l = fabr_tree_list_named(fabr_t_child(t, 0), "nada");

      ensure(l->size == 0);

      flu_list_free(l);
    }
  }

  describe "fabr_tree_collect()"
  {
    it "collects the trees that match the given function"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, 0, p);
      //char *st = fabr_tree_to_string(t, NULL); puts(st); free(st);

      fabr_tree **ts = fabr_tree_collect(t->child, is_value);

      ensure(ts[0] != NULL);
      ensure(ts[1] != NULL);
      ensure(ts[2] != NULL);
      ensure(ts[3] == NULL);

      ensure(ts[0]->name === "value");
      ensure(fabr_tree_string(s, ts[0]) ===f "1");
      ensure(ts[1]->name === "value");
      ensure(fabr_tree_string(s, ts[1]) ===f "2");
      ensure(ts[2]->name === "value");
      ensure(fabr_tree_string(s, ts[2]) ===f "3");

      ensure(fabr_tree_to_string(ts[0], NULL) ===f ""
        "[ \"value\", 1, 1, 1, null, \"alt-0\", [\n"
        "  [ \"number\", 1, 1, 1, null, \"rex-00\", [] ]\n"
        "] ]"
      );

      free(ts);
        // NB: only freeing this list of pointers,
        //     the trees themselves are freed in the "after each"
    }
  }

  describe "fabr_tree_string()"
  {
    it "returns a string containing the parsed input for the tree"
    {
      char *in = "[-1,0,1]";
      t = fabr_parse_all(in, 0, p);
      fabr_tree *tt = fabr_tree_lookup(t, "number");

      char *s = fabr_tree_string(in, tt);

      ensure(s === "-1");
      ensure(s != in + tt->offset);

      free(s);
    }

    it "returns an empty string when the tree is not a successful one"
    {
      char *in = "[nada]";
      t = fabr_parse_all(in, 0, p);

      char *s = fabr_tree_string(in, t);

      ensure(s === "");
      ensure(s != in + t->offset);

      free(s);
    }
  }

  describe "fabr_tree_str()"
  {
    it "returns a pointer to the beginning of the tree in the input"
    {
      char *in = "[-1,0,1]";
      t = fabr_parse_all(in, 0, p);
      fabr_tree *tt = fabr_tree_lookup(t, "number");

      char *s = fabr_tree_str(in, tt);

      ensure(s === "-1,0,1]");
      ensure(s == in + 1);
    }

    it "returns the pointer anyway if the tree is not a successful one"
    {
      char *in = "[nada]";
      t = fabr_parse_all(in, 0, p);

      char *s = fabr_tree_str(in, t);

      ensure(s === "[nada]");
      ensure(s == in);
    }
  }

  describe "fabr_tree_to_string() input == NULL"
  {
    it "returns a string representation of a fabr_tree"
    {
      t = fabr_parse_all("-1", 0, p);
      char *s = fabr_tree_to_string(t, NULL);

      //puts(s);
      ensure(s ===f ""
        "[ \"value\", 1, 0, 2, null, \"alt-0\", [\n"
        "  [ \"number\", 1, 0, 2, null, \"rex-00\", [] ]\n"
        "] ]");
    }
  }

  describe "fabr_tree_to_string() input != NULL"
  {
    it "returns a string representation of a fabr_tree with string leaves"
    {
      char *in = "[-1,0,1]";
      t = fabr_parse_all(in, 0, p);
      char *s = fabr_tree_to_string(t, in);

      //puts(s);
      ensure(s ===f ""
        "[ \"value\", 1, 0, 8, null, \"alt-0\", [\n"
        "  [ \"array\", 1, 0, 8, null, \"seq-01\", [\n"
        "    [ null, 1, 0, 1, null, \"string-010\", \"[\" ],\n"
        "    [ \"values\", 1, 1, 6, null, \"rep-011\", [\n"
        "      [ null, 1, 1, 6, null, \"seq-0110\", [\n"
        "        [ \"value\", 1, 1, 2, null, \"alt-0\", [\n"
        "          [ \"number\", 1, 1, 2, null, \"rex-00\", \"-1\" ]\n"
        "        ] ],\n"
        "        [ null, 1, 3, 4, null, \"rep-01101\", [\n"
        "          [ null, 1, 3, 2, null, \"seq-011010\", [\n"
        "            [ null, 1, 3, 1, null, \"string-0110100\", \",\" ],\n"
        "            [ \"value\", 1, 4, 1, null, \"alt-0\", [\n"
        "              [ \"number\", 1, 4, 1, null, \"rex-00\", \"0\" ]\n"
        "            ] ]\n"
        "          ] ],\n"
        "          [ null, 1, 5, 2, null, \"seq-011010\", [\n"
        "            [ null, 1, 5, 1, null, \"string-0110100\", \",\" ],\n"
        "            [ \"value\", 1, 6, 1, null, \"alt-0\", [\n"
        "              [ \"number\", 1, 6, 1, null, \"rex-00\", \"1\" ]\n"
        "            ] ]\n"
        "          ] ]\n"
        "        ] ]\n"
        "      ] ]\n"
        "    ] ],\n"
        "    [ null, 1, 7, 1, null, \"string-012\", \"]\" ]\n"
        "  ] ]\n"
        "] ]");
    }

    it "escapes the leave string"
    {
      char *in = "\"hello\nworld\"";
      t = fabr_parse_all(in, 0, p);
      char *s = fabr_tree_to_string(t, in);

      ensure(s ===f ""
        "[ \"value\", 1, 0, 13, null, \"alt-0\", [\n"
        "  [ \"string\", 1, 0, 13, null, \"rex-02\", \"\\\"hello\\nworld\\\"\" ]\n"
        "] ]");
    }
  }

  describe "fabr_tree_to_str()"
  {
    it "returns a string representation of a fabr_tree (without children)"
    {
      char *in = "-1";
      t = fabr_parse_all(in, 0, p);
      char *s = fabr_tree_to_str(t, in);

      //puts(s);
      ensure(s ===f "[ \"value\", 1, 0, 2, null, \"alt-0\", 1 ]");
    }

    it "says 0 if the input is not given and the tree has no children"
    {
      t = fabr_parse_all("-1", 0, p);
      fabr_tree *tt = t->child;

      char *s = fabr_tree_to_str(tt, NULL);
      ensure(s ===f "[ \"number\", 1, 0, 2, null, \"rex-00\", 0 ]");
    }

    it "displays the text if the input is given and the tree is a leaf"
    {
      char *in = "-1";
      t = fabr_parse_all(in, 0, p);
      fabr_tree *tt = t->child;

      char *s = fabr_tree_to_str(tt, in);
      ensure(s ===f "[ \"number\", 1, 0, 2, null, \"rex-00\", \"-1\" ]");
    }
  }

  describe "fabr_tree_lookup()"
  {
    it "returns NULL if it finds nothing"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, 0, p);

      //puts(fabr_tree_to_string(t, s));

      fabr_tree *r = fabr_tree_lookup(t, "string");

      ensure(r == NULL);
    }

    it "returns the first sub-tree with the given name"
    {
      char *s = "[1,\"deux\",3]";
      t = fabr_parse_all(s, 0, p);

      //puts(fabr_tree_to_string(t, s);

      fabr_tree *r = fabr_tree_lookup(t, "string");

      ensure(r != NULL);
      ensure(r->name === "string");
      ensure(r->offset == 3);
      ensure(r->length == 6);
    }
  }
}

