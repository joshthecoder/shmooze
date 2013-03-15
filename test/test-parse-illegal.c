// A set of tests to verify the parser can handle illegal
// messages by aborting and reporting error codes. These tests
// to should guard the parser from exploits such as buffer overflow
// by feeding it maliciously formatted messages.

#include "task.h"

#include "irc.h"

// Parse the message and returns the last error code.
static int parse_message(const char* msg) {
  irc_parser_t parser;
  irc_parser_init(&parser);
  irc_parser_execute(&parser, msg, strlen(msg));
  return parser.last_error;
}

// Messages cannot have whitespace as the first byte.
// This should cause an PARSER_ILLEGAL_TOKEN error code.
TEST_IMPL(parse_illegal_first_byte_whitespace) {
  ASSERT(parse_message(" BAD\r\n") == PARSER_ILLEGAL_TOKEN);
  return 0;
}

// Messages that exceed 512 bytes should cause the parser
// to abort and return error code PARSER_MESSAGE_TOO_LONG.
TEST_IMPL(parse_illegal_too_long) {
  const char* msg = ":Bob PRIVMSG John :";
  size_t msg_len = strlen(msg);
  char msg_end[1024];
  memset(&msg_end, 'a', 1024);

  irc_parser_t parser;
  irc_parser_init(&parser);

  // Seed the parser with the start of a valid message.
  ASSERT(irc_parser_execute(&parser, msg, msg_len) == msg_len);
  ASSERT(parser.last_error == 0);

  // Feed the parser enough valid data to go over the IRC message limit.
  // It should cause the parser to stop once it reaches 512 bytes parsed.
  ASSERT(irc_parser_execute(&parser, msg_end, 1024) == (1024 - msg_len));
  ASSERT(parser.last_error == PARSER_MESSAGE_TOO_LONG);
  ASSERT(parser.len == 512);

  return 0;
}

