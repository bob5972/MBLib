# meson.build -- MBLib
#    Q: Can anything be sadder than work left unfinished?
#    A: Yes, work never begun.

project('MBLib', 'c', 'cpp',
        default_options : 'default_library=static')

# Configuration
myconf = []
if host_machine.system() == 'linux'
    myconf += ['MB_LINUX']
elif host_machine.system() == 'darwin'
    myconf += ['MB_MACOS']
else
    error('Unknown host system:' + host_machine.system())
endif
if get_option('debug')
    myconf += ['MB_DEBUG']
endif
if get_option('devel')
    myconf += ['MB_DEVEL']
endif
conf_data = configuration_data()
foreach c : myconf
    conf_data.set(c, 1)
endforeach
configure_file(input : 'mbconfig.h.in',
               output : 'mbconfig.h',
               configuration : conf_data)

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
]

if get_option('devel')
    message('Enabling devel options')
    cargs += ['-Wall', '-Wextra', '-Werror']
    cargs += ['-Wno-attributes']
    cargs += ['-Wno-unused-parameter']
    cargs += ['-Wno-sign-compare']
    cargs += ['-Wno-unused-result']

    # Needs to be skipped on MacOS/Clang
    cargs += ['-Wno-format-truncation']
else
    message('Disabling devel options')
endif

if get_option('debug')
    message('Enabling debug options')
    cargs += ['-g', '-Og']
    cargs += ['-fno-omit-frame-pointer']
    cargs += ['-Wno-type-limits']
else
    message('Disabling debug options')
    cargs += ['-O2']
    cargs += ['-fomit-frame-pointer']
    cargs += ['-Wno-unused-variable']
    cargs += ['-Wno-unused-function']
endif

# Clang needs:
# -Wno-constant-logical-operand
# -Wno-unused-function

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
