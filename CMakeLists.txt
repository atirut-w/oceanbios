cmake_minimum_required(VERSION 3.20)
set(LLVM_MOS_PLATFORM common)
find_package(llvm-mos-sdk REQUIRED)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(OceanBIOS)
include_directories(src/include)
configure_file(link.ld link.ld COPYONLY)
add_compile_options(-O3 -linit-stack -lexit-loop)

add_executable(bios.bin src/main.c src/component.c src/terminal.c)
