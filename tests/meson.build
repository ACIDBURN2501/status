test_exe = executable(
  'test_status',
  ['test_status.c'],
  dependencies: [status_dep],
  c_args: ['-Werror'],
)

test('status module', test_exe)
