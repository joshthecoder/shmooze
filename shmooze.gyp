{
  'targets': [
    {
      'target_name': 'libshmooze',
      'type': '<(library)',
      'dependencies': [
        'deps/libuv/uv.gyp:libuv'
      ],
      'include_dirs': [
        'include'
      ],
      'sources': [
        'src/parser.c'
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
