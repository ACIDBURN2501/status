project('status', 'c', version: '0.1.0', default_options: ['c_std=c11'])

# Add DEBUG define only in debug builds
if get_option('buildtype') == 'debug'
  add_project_arguments('-DDEBUG', language: 'c')
endif

status_sources = files('src/status.c')

# Only install the real public header
version_split = meson.project_version().split('.')
major = version_split[0]
conf_data = configuration_data()
conf_data.set('MAJOR', major)
conf_data.set('MINOR', version_split[1])
conf_data.set('PATCH', version_split[2])

configure_file(
  input: 'src/status_version.h.in',
  output: 'status_version.h',
  configuration: conf_data,
)

status_headers = files('src/status.h')

status_inc = include_directories('src', '.')

status_lib = both_libraries(
  'status',
  status_sources,
  include_directories: status_inc,
  version: meson.project_version(),
  soversion: major,
)

status_dep = declare_dependency(
  include_directories: status_inc,
  link_with: status_lib,
  sources: status_headers,
)

subdir('tests')
