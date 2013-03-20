{
  'targets': [
    {
      'target_name': 'libshmooze',
      'type': '<(library)',
      'dependencies': [
        'deps/libuv/uv.gyp:libuv'
      ],
      'include_dirs': [
        'include',
        '>(SHARED_INTERMEDIATE_DIR)'
      ],
      'sources': [
        'src/command-lookup.gperf',
        'src/parser.c'
      ],
      'rules': [
        {
          'rule_name': 'gperf',
          'extension': 'gperf',
          'message': 'Generating lookup function from <(RULE_INPUT_PATH)',
          'outputs': [
            '<(SHARED_INTERMEDIATE_DIR)/<(RULE_INPUT_ROOT).c'
          ],
          'action': [
            'gperf',
            '<(RULE_INPUT_PATH)',
            '--output-file=<(SHARED_INTERMEDIATE_DIR)/<(RULE_INPUT_ROOT).c'
          ],
          'process_outputs_as_sources': 1
        }
      ]
    },

    {
      'target_name': 'run-shmooze-tests',
      'type': 'executable',
      'dependencies': [
        'libshmooze',
        'deps/libuv/uv.gyp:libuv'
      ],
      'include_dirs': [
        'include'
      ],
      'sources': [
        'test/run-tests.c',
        'test/runner-unix.c',
        'test/runner-unix.h',
        'test/runner.c',
        'test/runner.h',
        'test/task.h',
        'test/test-list.h',
        'test/test-parse-message.c',
        'test/test-parse-illegal.c'
      ]
    },

    {
      'target_name': 'run-shmooze-benchmarks',
      'type': 'executable',
      'dependencies': [
        'libshmooze',
        'deps/libuv/uv.gyp:libuv'
      ],
      'include_dirs': [
        'include'
      ],
      'sources': [
        'test/run-benchmarks.c',
        'test/runner-unix.c',
        'test/runner-unix.h',
        'test/runner.c',
        'test/runner.h',
        'test/task.h',
        'test/benchmark-list.h',
        'test/benchmark-parse-messages.c'
      ]
    }
  ]
}
