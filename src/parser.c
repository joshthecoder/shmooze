#include "irc.h"

enum parser_state {
  STATE_START,
  STATE_PREFIX,
  STATE_COMMAND,
  STATE_PARAMETER,
  STATE_MIDDLE,
  STATE_TRAILING,
  STATE_END
};

// IRC messages cannot exceed 512 bytes.
#define MESSAGE_MAX_LENGTH 512

/*
 * These macros are provided for accessing the 512 byte
 * buffer used by the parser for holding message parts.
 */
#define DATA_START(parser) parser->data
#define DATA_END(parser) parser->data + parser->len
#define DATA_APPEND(parser, b) parser->data[parser->len++] = b
#define DATA_NUL(parser) DATA_APPEND(parser, '\0')

void irc_parser_init(irc_parser_t* parser) {
  parser->last_error = 0;
  parser->state = STATE_START;
  parser->message_cb = 0;
}

int irc_parser_execute(irc_parser_t* parser, const char* buffer, size_t len) {
  // Counts the number of bytes parsed from the buffer.
  size_t bytes_parsed = 0;

  unsigned char state = parser->state;
  irc_message_t* message = &parser->message;

  while (bytes_parsed < len) {
  // The main parser loop which runs until buffer end is reached.
    // Get message character to parse and advance to next byte.
    char ch = *(buffer + bytes_parsed++);

    // Process the current message byte based on current state.
    switch (state) {
      case STATE_START:
        // Started parsing a new message.
        message->prefix = 0;
        message->parameter_count = 0;
        parser->len = 0;

        switch (ch) {
          case ':':
            // Message has a prefix we need to scan.
            state = STATE_PREFIX;
            message->prefix = DATA_END(parser);
            continue;

          case ' ':
            // Illegal to have first byte be a whitespace token.
            parser->last_error = PARSER_ILLEGAL_TOKEN;
            return bytes_parsed;
        }

        // If the message has no prefix, skip to parsing the command.
        message->command = DATA_END(parser);
        state = STATE_COMMAND;
        break;

      case STATE_PREFIX:
        // Prefixes are terminated by the first space.
        if (ch == ' ') {
          DATA_NUL(parser);
          state = STATE_COMMAND;
          message->command = DATA_END(parser);
          continue;
        }

        break;

      case STATE_COMMAND:
        // Command name terminated by first space or CR.
        switch (ch) {
          case ' ':
            // End of current parameter.
            state = STATE_PARAMETER;
            continue;

          case '\r':
            // End of message.
            state = STATE_END;
            continue;
        }

        break;

      case STATE_PARAMETER:
        DATA_NUL(parser);
        message->parameters[message->parameter_count++] = DATA_END(parser);

        if (ch == ':') {
          // Trailing parameter.
          state = STATE_TRAILING;
          continue;
        }

        // Middle parameter.
        state = STATE_MIDDLE;
        break;

      case STATE_MIDDLE:
        // Middle parameters terminated by space or CR.
        switch (ch) {
          case ' ':
            // Message has another parameter to be scanned.
            state = STATE_PARAMETER;
            continue;

          case '\r':
            // End of message.
            state = STATE_END;
            continue;
        }

        break;

      case STATE_TRAILING:
        // Trailing parameters terminated by CR.
        if (ch == '\r') {
          state = STATE_END;
          continue;
        }

        break;

      case STATE_END:
        DATA_NUL(parser);
        if (parser->message_cb && (parser->last_error = parser->message_cb(message))) {
          return bytes_parsed;
        }
        state = STATE_START;
        continue;
    }

    // Append current byte to the parser buffer if control reaches here.
    // These bytes will form the message parts (ex: prefix, command).
    DATA_APPEND(parser, ch);
  }

  parser->state = state;

  return bytes_parsed;
}

