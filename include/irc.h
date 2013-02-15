#ifndef UV_IRC_H
#define UV_IRC_H

#include "uv.h"

typedef struct irc_message_s irc_message_t;
typedef struct irc_parser_s irc_parser_t;

typedef enum {
  IRC_PASSWORD
} irc_command;

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

struct irc_parser_s {
  irc_message_cb message_cb;

  /*
   * The last error that caused the parser to stop.
   * If a non-zero value was returned from irc_message_cb
   * this variable will contain that value.
   */
  int last_error;

  // --- Private ---

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

#endif
