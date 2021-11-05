# meson.build -- MBLib
#    Q: Can anything be sadder than work left unfinished?
#    A: Yes, work never begun.

project('MBLib', 'c', 'cpp',
        default_options : 'default_library=static')

src = [
    'BitVector.c',
    'Dumper.cpp',
    'IntMap.c',
    'mbassert.c',
    'MBCompare.c',
    'mbdebug.c',
    'MBMatrix.cpp',
    'MBOpt.c',
    'MBQueue.cpp',
    'MBRegistry.c',
    'MBSet.cpp',
    'MBSocket.cpp',
    'MBStack.cpp',
    'MBString.c',
    'MBString.cpp',
    'MBVector.c',
    'MBVector.cpp',
    'random.c',
]

cargs = [
    '-D_GNU_SOURCE',
    '-march=native',
    '-Wall', '-Wextra', '-Werror',
    '-Wno-attributes',
    '-Wno-unused-parameter',
    '-Wno-sign-compare',
    '-Wno-format-truncation',
    '-Wno-unused-result',
]

if get_option('debug')
    cargs += ['-g', '-Og']
    cargs += ['-fno-omit-frame-pointer']
    cargs += ['-Wno-type-limits']
else
    cargs += ['-O2']
    cargs += ['-fomit-frame-pointer']
    cargs += ['-Wno-unused-variable']
    cargs += ['-Wno-unused-function']
endif

add_project_arguments(cargs, language : 'c')
add_project_arguments('--std=gnu11', language : 'c')
add_project_arguments(cargs, language : 'cpp')

lib = library('MBLib', sources : src,
              include_directories : 'public')

executable('test.bin', 'test.cpp',
           link_with : lib,
           include_directories : 'public')

executable('benchmark.bin', 'test.cpp',
           link_with : lib,
           include_directories : 'public',
           c_args : '-DBENCHMARK',
           cpp_args : '-DBENCHMARK')
