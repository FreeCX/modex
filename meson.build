project('modex', 'c',
    version: '0.1.0',
    default_options: ['werror=true', 'warning_level=3'])

include_dir = include_directories('src')
src_files = [
    'src/error.c',
    'src/memory.c',
    'src/storage.c'
]
static_modex = static_library('modex',
    sources: src_files,
    include_directories: include_dir)
library_modex = shared_library('modex',
    sources: src_files,
    include_directories: include_dir,
    gnu_symbol_visibility: 'hidden')

executable('demo', 'src/demo.c', link_with: static_modex)
