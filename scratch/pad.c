
abr_node *abr_parse_string(char *input, int offset, char *s);

abr_node *parse_json_string(char *input, int offset, char *s)
{
  return
    abr_parse_string(input, offset, "\"") &&
    abr_parse_regex(input, offset, "\"") &&
}

  string("x");
  or(string("x"), string("y")); // returns a parser_tree
  sequence(string("x"), regex("^nada"));
  string("name", "x")
    //
    // they have to be prefixed with abr_ somehow
    //
    // or invent a mini-language easily parseable?

abr_node // produce
  // should I rename it to abr_result_node?
abr_parse_node // productor

abr_parse_node {
  char *name;
  sig func;
  struct abr_parse_node *children;
}

abr_node *abr_parse(abr_parse_node *root, char *input); // signature

// ***

// __func__ (c99)
// printf("func is >%s<\n", __func__);

abr_node *abr_parse_string(abr_input *i, char *name, ...);
void abr_parse_string(abr_context *c, char *name, ...);
void abr_parse_string(abr_context *c, ...);
void abr_name(abr_context *c, ...);

typedef void abr_parser(abr_context *c, ...);

// but
abr_parse_or(c, my_parse_x, my_parse_y, NULL);
abr_parse_or(c, my_parse_x, my_parse_y, abr_parse_over);
// "moindre mal" ?


//    parser do
//
//      value ==
//        spaces? +
//        (object | array | string | number | btrue | bfalse | null) +
//        spaces?
//
//      spaces? == _("\s\n\r") * 0
//
//      object == `{` + (entry + (`,` + entry) * 0) * 0 + `}`
//      entry == spaces? + string + spaces? + `:` + value
//
//      array == `[` + (value + (`,` + value) * 0) * 0 + `]`
//
//      string == `"` + ((`\\` + _) | _('^"')) * 0 + `"`
//
//      _digit == _("0-9")
//
//      number ==
//        `-` * -1 +
//        (`0` | (_("1-9") + _digit * 0)) +
//        (`.` + _digit * 1) * -1 +
//        (_("eE") + _("+-") * -1 + _digit * 1) * -1
//
//      btrue == `true`
//      bfalse == `false`
//      null == `null`
//    end
void my_parse_val(abr_context *c, ...)
{
  abr_parse_or(
    c,
    my_parse_object,
    my_parse_array,
    my_parse_string,
    // ...
    NULL);
}
void my_parse_value(abr_context *c, ...)
{
  abr_parse_sequence(
    c,
    my_parse_optional_spaces,
    my_parse_val,
    my_parse_optional_spaces,
    NULL);
}
// circularity ?

abr_p_sequence / abr_p_alternative
abr_p_and / abr_p_or

abr_p_char
abr_p_string
abr_p_regex

abr_p_repetition
abr_p_not

abr_p_name

abr_p_lookahead //presence / absence

// ...

void my_parse_xs(char *s, int offset, ...)
{
  return abr_p_repetition(*s, offset, 0, 1, my_parse_string);
}
// => we have to define a method for anything useful...
// => we're better served by compiled a parser tree
//    (as opposed to a parse tree)

