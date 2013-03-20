// A set of tests to verify valid IRC messages are parsed correctly.

#include "irc.h"

#include "task.h"

#include <string.h>

#define SETUP_PARSER(cb) \
  irc_parser_t parser; \
  irc_parser_init(&parser); \
  parser.message_cb = cb;

#define PARSER_EXECUTE(msg) \
  size_t msg_len = strlen(msg); \
  ASSERT(irc_parser_execute(&parser, msg, msg_len) == msg_len); \
  ASSERT(parser.last_error == 0);

// Command message.
static const char* command_message = "PING\r\n";

static int command_message_cb(irc_message_t* msg) {
  ASSERT(msg->prefix == 0);
  ASSERT(strcmp(msg->command, "PING") == 0);
  ASSERT(msg->parameter_count == 0);
  return 0;
}

TEST_IMPL(parse_message_command) {
  SETUP_PARSER(command_message_cb);
  PARSER_EXECUTE(command_message);
  return 0;
}


// Prefix message.
const char* prefix_message = ":Wiz VERSION\r\n";

static int prefix_message_cb(irc_message_t* msg) {
  ASSERT(strcmp(msg->prefix, "Wiz") == 0);
  ASSERT(strcmp(msg->command, "VERSION") == 0);
  ASSERT(msg->parameter_count == 0);
  return 0;
}

TEST_IMPL(parse_message_prefix) {
  SETUP_PARSER(prefix_message_cb);
  PARSER_EXECUTE(prefix_message);
  return 0;
}

// Middle parameters.
static const char* middle_message = "JOIN #foo,#bar fubar,foobar\r\n";

static int middle_message_cb(irc_message_t* msg) {
  ASSERT(strcmp(msg->command, "JOIN") == 0);
  ASSERT(msg->parameter_count == 2);
  ASSERT(strcmp(msg->parameters[0], "#foo,#bar") == 0);
  ASSERT(strcmp(msg->parameters[1], "fubar,foobar") == 0);
  return 0;
}

TEST_IMPL(parse_message_middle) {
  SETUP_PARSER(middle_message_cb);
  PARSER_EXECUTE(middle_message);
  return 0;
}


// Trailing parameters.
const char* trailing_message = "PRIVMSG John :Hello, World!\r\n";

static int trailing_message_cb(irc_message_t* msg) {
  ASSERT(strcmp(msg->command, "PRIVMSG") == 0);
  ASSERT(msg->parameter_count == 2);
  ASSERT(strcmp(msg->parameters[0], "John") == 0);
  ASSERT(strcmp(msg->parameters[1], "Hello, World!") == 0);
  return 0;
}

TEST_IMPL(parse_message_trailing) {
  SETUP_PARSER(trailing_message_cb);
  PARSER_EXECUTE(trailing_message);
  return 0;
}

// Mixed message that tests all parts (prefix, command, parameters).
const char* mixed_message = ":Angel PRIVMSG Wiz :Hello are you receiving this message ?\r\n";

static int mixed_message_cb(irc_message_t* msg) {
  ASSERT(strcmp(msg->prefix, "Angel") == 0);
  ASSERT(strcmp(msg->command, "PRIVMSG") == 0);
  ASSERT(msg->parameter_count == 2);
  ASSERT(strcmp(msg->parameters[0], "Wiz") == 0);
  ASSERT(strcmp(msg->parameters[1], "Hello are you receiving this message ?") == 0);
  return 0;
}

TEST_IMPL(parse_message_mixed) {
  SETUP_PARSER(mixed_message_cb);
  PARSER_EXECUTE(mixed_message);
  return 0;
}

TEST_IMPL(parse_command) {
  static const char *pass = "PASS",
                    *pass_lc = "pass",
                    *unknown = "FOOBAR";
  ASSERT(irc_parse_command(pass, strlen(pass)) == IRC_PASS);
  ASSERT(irc_parse_command(pass_lc, strlen(pass_lc)) == IRC_PASS);
  ASSERT(irc_parse_command(unknown, strlen(unknown)) == IRC_UNKNOWN);
  return 0;
}

