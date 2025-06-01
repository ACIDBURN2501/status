project(
  'status',
  'c',
  version: '0.1.0',
  default_options: ['c_std=c11']
)

# Add DEBUG define only in debug builds
if get_option('buildtype') == 'debug'
  add_project_arguments('-DDEBUG', language: 'c')
endif

status_sources = files('src/status.c')

# Only install the real public header
status_headers = files('include/status.h')

status_inc = include_directories('include')

status_lib = static_library(
  'status',
  status_sources,
  include_directories: status_inc,
  pic: true
)

status_dep = declare_dependency(
  include_directories: status_inc,
  link_with: status_lib,
  sources: status_headers
)

test_exe = executable(
  'test_status',
  ['tests/test_status.c'],
  dependencies: [status_dep],
  include_directories: status_inc
)

test('status module', test_exe)
