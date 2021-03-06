
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
  }
  after each
  {
    fabr_tree_free(t);
  }

  static fabr_tree *_comma(fabr_input *i) { return fabr_str(NULL, i, ","); }
  static fabr_tree *_osb(fabr_input *i) { return fabr_str(NULL, i, "["); }
  static fabr_tree *_csb(fabr_input *i) { return fabr_str(NULL, i, "]"); }
  //
  static fabr_tree *_value(fabr_input *i); // forward
  //
  static fabr_tree *_number(fabr_input *i)
  {
    return fabr_rex("number", i, "-?[0-9]+(\\.[0-9]+)?");
  }
  static fabr_tree *_string(fabr_input *i)
  {
    return fabr_rex(
      "string", i,
      "\"("
        "\\\\[\"/\\\\bfnrt]" "|"
        "\\\\u[0-9a-fA-F]{4}" "|"
        "[^\"\\\\]"
      ")*\"");
  }
  //static fabr_tree *_values(fabr_input *i)
  //{
  //  return fabr_jseq("values", i, _value, _comma);
  //}
  static fabr_tree *_array(fabr_input *i)
  {
    //return fabr_seq("array", i, _osb, _values, _csb, NULL);
    return fabr_eseq("array", i, _osb, _value, _comma, _csb);
  }
  static fabr_tree *_value(fabr_input *i)
  {
    return fabr_alt("value", i, _number, _array, _string, NULL);
  }

  describe "the sample _value parser"
  {
    it "accepts \"deux\""
    {
      char *s = "\"deux\"";

      t = fabr_parse_all(s, _value);
      //t = fabr_parse_f(s, _value, 0);

      //fabr_puts_tree(t, s, 1);

      expect(fabr_tree_to_string(t, s, 0) ===f ""
        "[ null, 1, 0, 6, null, \"all\", 0, [\n"
        "  [ \"value\", 1, 0, 6, null, \"alt\", 0, [\n"
        "    [ \"string\", 1, 0, 6, null, \"rex\", 43, \"\\\"deux\\\"\" ]\n"
        "  ] ]\n"
        "] ]");
    }
  }

  describe "fabr_tree_to_string()"
  {
    it "returns \"null\" if the tree is NULL"
    {
      char *s = fabr_tree_to_string(NULL, NULL, 0);

      expect(s ===f "null");
    }
  }

  //typedef int fabr_tree_func(fabr_tree *);
  //
  static short is_value(const fabr_tree *t)
  {
    // -1: fail, do not continue
    //  0: fail, please check my children
    //  1: success

    //if (t == NULL) return -1; // should not happen
    if (t->result != 1) return -1;
    return t->name && strcmp(t->name, "value") == 0;
  }

  describe "fabr_tree_list()"
  {
    it "collects the trees that match the given function"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      flu_list *l = fabr_tree_list(fabr_t_path(t, 0, 0, -1), is_value);

      ensure(l->size zu== 3);

      fabr_tree *t0 = (fabr_tree *)flu_list_at(l, 0);
      fabr_tree *t1 = (fabr_tree *)flu_list_at(l, 1);
      fabr_tree *t2 = (fabr_tree *)flu_list_at(l, 2);

      ensure(t0->name === "value");
      ensure(fabr_tree_string(s, t0) ===f "1");
      ensure(fabr_tree_llong(s, t0, 10) == 1);
      ensure(t1->name === "value");
      ensure(fabr_tree_string(s, t1) ===f "2");
      ensure(t2->name === "value");
      ensure(fabr_tree_string(s, t2) ===f "3");

      ensure(fabr_tree_to_string(t0, s, 0) ===f ""
        "[ \"value\", 1, 1, 1, null, \"alt\", 0, [\n"
        "  [ \"number\", 1, 1, 1, null, \"rex\", 19, \"1\" ]\n"
        "] ]"
      );

      flu_list_free(l);
        // NB: only freeing this list of pointers,
        //     the trees themselves are freed in the "after each"
    }

    it "returns NULL when the tree is NULL"
    {
      flu_list *l = fabr_tree_list(NULL, is_value);

      ensure(l == NULL);
    }
  }

  describe "fabr_tree_list_cn()"
  {
    it "collects the trees that match the given function"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      flu_list *l = fabr_tree_list_cn(t->child, is_value);

      ensure(l->size zu== 3);

      fabr_tree *t0 = (fabr_tree *)flu_list_at(l, 0);
      fabr_tree *t1 = (fabr_tree *)flu_list_at(l, 1);
      fabr_tree *t2 = (fabr_tree *)flu_list_at(l, 2);

      ensure(t0->name === "value");
      ensure(fabr_tree_string(s, t0) ===f "1");
      ensure(t1->name === "value");
      ensure(fabr_tree_string(s, t1) ===f "2");
      ensure(t2->name === "value");
      ensure(fabr_tree_string(s, t2) ===f "3");

      ensure(fabr_tree_to_string(t0, s, 0) ===f ""
        "[ \"value\", 1, 1, 1, null, \"alt\", 0, [\n"
        "  [ \"number\", 1, 1, 1, null, \"rex\", 19, \"1\" ]\n"
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
      t = fabr_parse_all(s, _value);
      //char *st = fabr_tree_to_string(t, NULL, 1); puts(st); free(st);

      flu_list *l = fabr_tree_list_named(fabr_t_path(t, 0, 0, -1), "value");

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

    it "returns NULL when the tree is NULL"
    {
      flu_list *l = fabr_tree_list_named(NULL, "nada");

      ensure(l == NULL);
    }

    it "returns an empty list when it finds nothing"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      flu_list *l = fabr_tree_list_named(fabr_t_path(t, 0, 0, -1), "nada");

      ensure(l->size == 0);

      flu_list_free(l);
    }

    it "returns nodes that have names when given NULL"
    {
      char *s = "[1,\"deux\",3]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      flu_list *l = fabr_tree_list_named(t, NULL);
      ensure(l->size zu== 1);
      ensure(((fabr_tree *)l->first->item)->name === "value");
      flu_list_free(l);

      l = fabr_tree_list_named(t->child->child, NULL);
      ensure(l->size zu== 1);
      ensure(((fabr_tree *)l->first->item)->name === "array");
      flu_list_free(l);

      l = fabr_tree_list_named(t->child->child->child, NULL);
      ensure(l->size zu== 0);
      flu_list_free(l);
    }
  }

  describe "fabr_tree_list_named_cn()"
  {
    it "returns nodes that have names when given NULL"
    {
      char *s = "[1,\"deux\",3]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      flu_list *l = fabr_tree_list_named_cn(t, NULL);
      ensure(l->size zu== 1);
      ensure(((fabr_tree *)l->first->item)->name === "value");
      flu_list_free(l);

      l = fabr_tree_list_named_cn(t->child->child, NULL);
      ensure(l->size zu== 3);
      ensure(((fabr_tree *)l->first->item)->name === "value");
      flu_list_free(l);
    }
  }

  describe "fabr_tree_collect()"
  {
    it "collects the trees that match the given function"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      fabr_tree **ts = fabr_tree_collect(fabr_t_path(t, 0, 0, -1), is_value);

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

      ensure(fabr_tree_to_string(ts[0], s, 0) ===f ""
        "[ \"value\", 1, 1, 1, null, \"alt\", 0, [\n"
        "  [ \"number\", 1, 1, 1, null, \"rex\", 19, \"1\" ]\n"
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
      t = fabr_parse_all(in, _value);
      //fabr_puts_tree(t, in, 1);

      fabr_tree *tt = fabr_tree_lookup(t, "number");
      //fabr_puts_tree(t, in, 1);

      char *s = fabr_tree_string(in, tt);

      ensure(s === "-1");
      ensure(s != in + tt->offset);

      free(s);
    }

    it "returns an empty string when the tree is not a successful one"
    {
      char *in = "[nada]";
      t = fabr_parse_all(in, _value);
      //fabr_puts_tree(t, in, 1);

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
      t = fabr_parse_all(in, _value);
      fabr_tree *tt = fabr_tree_lookup(t, "number");

      char *s = fabr_tree_str(in, tt);

      ensure(s === "-1,0,1]");
      ensure(s == in + 1);
    }

    it "returns the pointer anyway if the tree is not a successful one"
    {
      char *in = "[nada]";
      t = fabr_parse_all(in, _value);

      char *s = fabr_tree_str(in, t);

      ensure(s === "[nada]");
      ensure(s == in);
    }
  }

  describe "fabr_tree_to_string()"
  {
    context "input == NULL"
    {
      it "returns a string representation of a fabr_tree"
      {
        char *in = "-1";

        t = fabr_parse_all(in, _value);
        char *s = fabr_tree_to_string(t, in, 0);

        ensure(s ===f ""
          "[ null, 1, 0, 2, null, \"all\", 0, [\n"
          "  [ \"value\", 1, 0, 2, null, \"alt\", 0, [\n"
          "    [ \"number\", 1, 0, 2, null, \"rex\", 19, \"-1\" ]\n"
          "  ] ]\n"
          "] ]");
      }
    }

    context "input != NULL"
    {
      it "returns a string representation of a fabr_tree with string leaves"
      {
        char *in = "[-1,0,1]";
        t = fabr_parse_all(in, _value);

        ensure(fabr_tree_to_string(t, in, 0) ===f ""
          "[ null, 1, 0, 8, null, \"all\", 0, [\n"
          "  [ \"value\", 1, 0, 8, null, \"alt\", 0, [\n"
          "    [ \"array\", 1, 0, 8, null, \"eseq\", 0, [\n"
          "      [ null, 1, 0, 1, null, \"str\", 1, \"[\" ],\n"
          "      [ \"value\", 1, 1, 2, null, \"alt\", 0, [\n"
          "        [ \"number\", 1, 1, 2, null, \"rex\", 19, \"-1\" ]\n"
          "      ] ],\n"
          "      [ null, 1, 3, 1, null, \"str\", 1, \",\" ],\n"
          "      [ \"value\", 1, 4, 1, null, \"alt\", 0, [\n"
          "        [ \"number\", 1, 4, 1, null, \"rex\", 19, \"0\" ]\n"
          "      ] ],\n"
          "      [ null, 1, 5, 1, null, \"str\", 1, \",\" ],\n"
          "      [ \"value\", 1, 6, 1, null, \"alt\", 0, [\n"
          "        [ \"number\", 1, 6, 1, null, \"rex\", 19, \"1\" ]\n"
          "      ] ],\n"
          "      [ null, 1, 7, 1, null, \"str\", 1, \"]\" ]\n"
          "    ] ]\n"
          "  ] ]\n"
          "] ]");
      }

      it "escapes the leaf string"
      {
        char *in = "\"hello\nworld\"";

        t = fabr_parse_all(in, _value);

        ensure(fabr_tree_to_string(t, in, 0) ===f ""
          "[ null, 1, 0, 13, null, \"all\", 0, [\n"
          "  [ \"value\", 1, 0, 13, null, \"alt\", 0, [\n"
          "    [ \"string\", 1, 0, 13, null, \"rex\", 43, \"\\\"hello\\nworld\\\"\" ]\n"
          "  ] ]\n"
          "] ]");
      }
    }
  }

  describe "fabr_tree_to_str()"
  {
    it "returns a string representation of a fabr_tree (without children)"
    {
      char *in = "-1";
      t = fabr_parse_all(in, _value);

      ensure(fabr_tree_to_str(t->child, in, 0) ===f ""
        "[ \"value\", 1, 0, 2, null, \"alt\", 0, 1 ]");
    }

    it "says 0 if the input is not given and the tree has no children"
    {
      char *in = "-1";
      t = fabr_parse_all(in, _value);

      ensure(fabr_tree_to_str(t->child->child, NULL, 0) ===f ""
        "[ \"number\", 1, 0, 2, null, \"rex\", 19, 0 ]");
    }

    it "displays the text if the input is given and the tree is a leaf"
    {
      char *in = "-1";
      t = fabr_parse_all(in, _value);

      ensure(fabr_tree_to_str(t->child->child, in, 0) ===f ""
        "[ \"number\", 1, 0, 2, null, \"rex\", 19, \"-1\" ]");
    }
  }

  describe "fabr_tree_lookup()"
  {
    context "(t, subTreeName)"
    {
      it "returns NULL if it finds nothing"
      {
        char *s = "[1,2,3]";
        t = fabr_parse_all(s, _value);

        fabr_tree *r = fabr_tree_lookup(t, "string");

        ensure(r == NULL);
      }

      it "returns the first tree with the given name"
      {
        char *s = "[1,\"deux\",3]";
        t = fabr_parse_all(s, _value);
        //fabr_puts_tree(t, s, 1);

        fabr_tree *r = fabr_tree_lookup(t, "string");

        ensure(r != NULL);
        ensure(r->name === "string");
        ensure(r->offset == 3);
        ensure(r->length == 6);
      }
    }

    context "(t, NULL)"
    {
      it "returns NULL if it finds nothing"
      {
        char *s = "yyy";
        t = fabr_parse_all(s, _value);
        //fabr_puts_tree(t, s, 1);

        expect(fabr_tree_lookup(t->child->child->child, NULL) == NULL);
      }

      it "returns the first tree with a [non-NULL] name"
      {
        char *s = "[1,\"deux\",3]";
        t = fabr_parse_all(s, _value);
        //fabr_puts_tree(t, s, 1);

        fabr_tree *tt = NULL;

        tt = fabr_tree_lookup(t, NULL);

        expect(fabr_tree_to_str(tt, s, 0) ===f ""
          "[ \"value\", 1, 0, 12, null, \"alt\", 0, 1 ]");

        tt = fabr_tree_lookup(t->child->child, NULL);

        expect(fabr_tree_to_str(tt, s, 0) ===f ""
          "[ \"array\", 1, 0, 12, null, \"eseq\", 0, 7 ]");
      }
    }
  }

  describe "fabr_subtree_lookup()"
  {
    it "returns NULL if it finds nothing"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, _value);

      fabr_tree *r = fabr_subtree_lookup(t, "string");

      expect(r == NULL);
    }

    it "returns the first sub-tree with the given name"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      fabr_tree *tt = fabr_subtree_lookup(t->child, "value");

      expect(tt != NULL);

      expect(fabr_tree_to_str(tt, s, 0) ===f ""
        "[ \"value\", 1, 1, 1, null, \"alt\", 0, 1 ]");
    }

    it "returns the first sub-tree with a[ny] name when name=NULL"
    {
      char *s = "[1,2,3]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      fabr_tree *tt = fabr_subtree_lookup(t->child, NULL);
      //fabr_puts_tree(tt, s, 1);

      expect(tt != NULL);

      expect(fabr_tree_to_str(tt, s, 0) ===f ""
        "[ \"array\", 1, 0, 7, null, \"eseq\", 0, 7 ]");
    }
  }

  describe "fabr_lookup_string()"
  {
    it "returns NULL if it finds nothing"
    {
      char *s = "[-1,0,1]";
      t = fabr_parse_all(s, _value);

      expect(fabr_lookup_string(s, t, "nada") == NULL);
    }

    it "returns the [malloc'ed] string value for the given name"
    {
      char *s = "[-1,0,1]";
      t = fabr_parse_all(s, _value);
      //fabr_puts_tree(t, s, 1);

      expect(fabr_lookup_string(s, t, "array") ===f "[-1,0,1]");
      expect(fabr_lookup_string(s, t, "number") ===f "-1");
    }
  }

  describe "fabr_tree_llong()"
  {
    it "returns a long long containing the parsed input for the tree"
    {
      char *in = "[-1,0,1]";
      t = fabr_parse_all(in, _value);
      //fabr_puts_tree(t, in, 1);

      fabr_tree *tt = fabr_tree_lookup(t, "number");
      //fabr_puts_tree(t, in, 1);

      long long l = fabr_tree_llong(in, tt, 10);

      ensure(l == -1);
    }
}

