cmake_minimum_required(VERSION 3.20)
set(LLVM_MOS_PLATFORM common)
find_package(llvm-mos-sdk REQUIRED)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(OceanBIOS)
add_executable(bios.bin src/main.c src/component.c src/terminal.c src/interrupt.c)
configure_file(link.ld link.ld COPYONLY)

target_compile_options(bios.bin PRIVATE -O3)
target_include_directories(bios.bin PRIVATE src/include)
target_link_libraries(bios.bin init-stack exit-loop)
