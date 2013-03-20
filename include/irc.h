#ifndef UV_IRC_H
#define UV_IRC_H

#include "uv.h"

typedef struct irc_message_s irc_message_t;
typedef struct irc_parser_s irc_parser_t;

struct irc_message_s {
  const char* prefix;
  const char* command;
  const char* parameters[15];
  int parameter_count;
};

/*
 * Called when a new IRC message has been parsed.
 * Return a non-zero value to stop the parser.
 */
typedef int (*irc_message_cb)(irc_message_t* msg);

// An illegal token was encountered while parsing message.
#define PARSER_ILLEGAL_TOKEN 0x01

// The message has exceeded the 512 byte length limit.
#define PARSER_MESSAGE_TOO_LONG 0x02

struct irc_parser_s {
  irc_message_cb message_cb;

  // If an error occurred while parsing a message
  // this will contain a non-zero error code.
  // All errors are fatal and the connection providing
  // the data should be terminated. The parser may only
  // be executed again if re-initialized to clear the error.
  int last_error;

  // Private.
  unsigned char state;
  char data[512];
  int len;
  irc_message_t message;
};

// Initialize an IRC message parser instance.
void irc_parser_init(irc_parser_t* parser);

// Execute parser on the buffer of data invoking the
// callback for each fully parsed message. Returns
// the number of bytes parsed.
int irc_parser_execute(irc_parser_t* parser, const char* buffer, size_t len);

typedef enum {
  IRC_PASS,
  IRC_NICK,
  IRC_USER,
  IRC_SERVER,
  IRC_OPER,
  IRC_QUIT,
  IRC_SQUIT,
  IRC_JOIN,
  IRC_PART,
  IRC_MODE,
  IRC_TOPIC,
  IRC_NAMES,
  IRC_LIST,
  IRC_INVITE,
  IRC_KICK,
  IRC_VERSION,
  IRC_STATS,
  IRC_LINKS,
  IRC_TIME,
  IRC_CONNECT,
  IRC_TRACE,
  IRC_ADMIN,
  IRC_INFO,
  IRC_PRIVMSG,
  IRC_NOTICE,
  IRC_WHO,
  IRC_WHOIS,
  IRC_WHOWAS,
  IRC_KILL,
  IRC_PING,
  IRC_PONG,
  IRC_ERROR,
  IRC_AWAY,
  IRC_REHASH,
  IRC_RESTART,
  IRC_SUMMON,
  IRC_USERS,
  IRC_WALLOPS,
  IRC_USERHOST,
  IRC_ISON,
  IRC_UNKNOWN
} irc_command;

// Parse the command type from a buffer containing text.
irc_command irc_parse_command(const char* buffer, size_t len);

#endif
