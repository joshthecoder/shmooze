#ifndef UV_IRC_H
#define UV_IRC_H

#include "uv.h"

typedef struct irc_message_s irc_message_t;
typedef struct irc_parser_s irc_parser_t;

struct irc_message_s {
  const char* prefix;
  const char* command;
  const char* parameters[10];
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
  int data_len;
  irc_message_t message;
  int msg_len;
};

// Initialize an IRC message parser instance.
void irc_parser_init(irc_parser_t* parser);

// Execute parser on the buffer of data invoking the
// callback for each fully parsed message. Returns
// the number of bytes parsed.
int irc_parser_execute(irc_parser_t* parser, const char* buffer, size_t len);

#endif
