#include "task.h"

#include "irc.h"
#include "uv.h"

static const char* msg = ":testnick USER guest tolmoon tolsun :Ronnie Reagan\r\n";

static int total_count = 100000;  // total number of times to parse message.
static int parse_count = 0;       // number of messages parsed successfully.

static int message_cb(irc_message_t* msg) {
  parse_count++;
  return 0;
}

BENCHMARK_IMPL(parse_messages) {
  uv_loop_t* loop = uv_default_loop();

  irc_parser_t parser;
  irc_parser_init(&parser);
  parser.message_cb = message_cb;

  size_t msg_len = strlen(msg);

  uv_update_time(loop);
  int64_t start_time = uv_now(loop);

  for (int i = 0; i < total_count; i++) {
    irc_parser_execute(&parser, msg, msg_len);
  }

  uv_update_time(loop);
  int64_t end_time = uv_now(loop);

  ASSERT(parse_count == total_count);

  LOGF("Parsed %i messages in %lld ms\n", parse_count, end_time - start_time);
  LOGF("       %.0f messages/s\n",
       (double) parse_count / (double) (end_time - start_time) * 1000);

  MAKE_VALGRIND_HAPPY();
  return 0;
}

