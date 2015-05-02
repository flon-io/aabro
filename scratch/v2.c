
static void fshv_init_uri_parser()
{
  fabr_parser *scheme =
    fabr_n_rex("scheme", "https?");
  fabr_parser *host =
    fabr_n_rex("host", "[^:/]+");
  fabr_parser *port =
    fabr_seq(fabr_string(":"), fabr_n_rex("port", "[1-9][0-9]+"), NULL);

  fabr_parser *path =
    fabr_n_rex("path", "[^\\?#]+");
  fabr_parser *quentry =
    fabr_n_seq("quentry",
      fabr_n_rex("key", "[^=&#]+"),
      fabr_seq(fabr_string("="), fabr_n_rex("val", "[^&#]+"), fabr_r("?")),
      NULL);
  fabr_parser *query =
    fabr_n_seq("query",
      quentry,
      fabr_seq(fabr_string("&"), quentry, fabr_r("*")),
      NULL);
  fabr_parser *fragment =
    fabr_n_rex("fragment", ".+");

  fabr_parser *shp =
    fabr_seq(
      scheme,
      fabr_string("://"),
      host,
      port, fabr_q("?"),
      NULL);

  uri_parser =
    fabr_seq(
      shp, fabr_q("?"),
      path,
      fabr_seq(fabr_string("?"), query, fabr_r("?")),
      fabr_seq(fabr_string("#"), fragment, fabr_r("?")),
      NULL);
}

  // ...becomes...

fabr_tree *_scheme(char *s, size_t off)
{
  return fabr_rex("scheme", s, off, "https?");
}
fabr_tree *_host(char *s, size_t off)
{
  return fabr_rex("host", s, off, "[^:/]+");
}
fabr_tree *_port(char *s, size_t off)
{
  return fabr_rex("port", s, off, "[1-9][0-9]+");
}
fabr_tree *_path(char *s, size_t off)
{
  return fabr_rex("path", s, off, "[^\\?#]+");
}
fabr_tree *_qkey(char *s, size_t off)
{
  return fabr_rex("key", s, off, "[^=&#]+");
}
fabr_tree *_qequal(char *s, size_t off)
{
  return fabr_str(NULL, s, off, "=");
}
fabr_tree *_qval(char *s, size_t off)
{
  return fabr_rex("val", s, off, "[^&#]+");
}
fabr_tree *_qfullentry(char *s, size_t off)
{
  return fabr_seq(NULL, s, off, _qkey, _qequal, _qeval, NULL);
}
fabr_tree *_qentry(char *s, size_t off)
{
  return fabr_alt("qentry", s, off, _qfullentry, _qkey, NULL);
}
fabr_tree *_amp(char *s, size_t off)
{
  return fabr_str(NULL, s, off, "&");
}
fabr_tree *_query(char *s, size_t off)
{
  return fabr_jrep("query", s, off, _qentry, _amp, 0, -1);
}
fabr_tree *_fragment(char *s, size_t off)
{
  return fabr_rex("fragment", ,s, off, ".+");
}
  //fabr_parser *shp =
  //  fabr_seq(
  //    scheme,
  //    fabr_string("://"),
  //    host,
  //    port, fabr_q("?"),
  //    NULL);

  //uri_parser =
  //  fabr_seq(
  //    shp, fabr_q("?"),
  //    path,
  //    fabr_seq(fabr_string("?"), query, fabr_r("?")),
  //    fabr_seq(fabr_string("#"), fragment, fabr_r("?")),
  //    NULL);

