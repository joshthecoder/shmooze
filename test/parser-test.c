#include "irc.h"

#include <stdio.h>
#include <string.h>

const char* msg = ":foo bar\r\n";

int on_message(irc_message_t* msg) {
  printf("prefix = %s command = %s\n", msg->prefix, msg->command);
  return 0;
}

int main() {
  irc_parser_t parser;

  uv_buf_t buf = uv_buf_init((char*)msg, strlen(msg));

  irc_parser_init(&parser);
  parser.message_cb = on_message;
  irc_parser_execute(&parser, buf);

  return 0;
}

