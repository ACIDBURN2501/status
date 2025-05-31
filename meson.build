project(
  'status',
  'c',
  version: '0.1.0',
  default_options: ['c_std=c11']
)

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
