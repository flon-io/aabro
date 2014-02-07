
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

