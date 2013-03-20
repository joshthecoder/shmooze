TEST_DECLARE   (parse_message_command)
TEST_DECLARE   (parse_message_prefix)
TEST_DECLARE   (parse_message_middle)
TEST_DECLARE   (parse_message_trailing)
TEST_DECLARE   (parse_message_mixed)
TEST_DECLARE   (parse_command)

TEST_DECLARE   (parse_illegal_first_byte_whitespace)

TASK_LIST_START
  TEST_ENTRY  (parse_message_command)
  TEST_ENTRY  (parse_message_prefix)
  TEST_ENTRY  (parse_message_middle)
  TEST_ENTRY  (parse_message_trailing)
  TEST_ENTRY  (parse_message_mixed)
  TEST_ENTRY  (parse_command)

  TEST_ENTRY  (parse_illegal_first_byte_whitespace)
TASK_LIST_END
