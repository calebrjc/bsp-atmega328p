project(
    'bsp-atmega328p',
    'c',
    default_options: [
        'buildtype=minsize',
        'b_lto=true',
        'cpp_rtti=false',
        'cpp_eh_=none',
        'default_library=static'
    ],
)

assert(meson.is_cross_build(), 'This project can only be built in a cross build environment.')

printf_dep = dependency('printf', required: true)

bsp_atmega328p_inc = include_directories('include')

bsp_atmega328p_src = files(
    'src/dsa/queue.c',
    'src/io.c',
    'src/usart.c',
    'src/util/assert.c',
)

bsp_atmega328p_lib = library(
    'bsp',
    include_directories: bsp_atmega328p_inc,
    sources: bsp_atmega328p_src,
    dependencies: [printf_dep],
)

bsp_atmega328p_dep = declare_dependency(
    include_directories: bsp_atmega328p_inc,
    link_with: bsp_atmega328p_lib,
)
