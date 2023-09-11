cmake_minimum_required(VERSION 3.5)

project(CHIP-8 VERSION 0.0.1)

set(SOURCES_CHIP_8 "./chip8/Chip8.cpp" "./chip8/romLoaderCH8.cpp")
set(SOURCES_UTILS "./utils/Radix.cpp")
set(SOURCES_IO "./window/Window_SFML.cpp")

set(SFML_DIR ./SFML/lib/cmake/SFML)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(Chip8 main.cpp ${SOURCES_IO}  ${SOURCES_CHIP_8} ${SOURCES_UTILS} )

include_directories("./chip8" "./utils" "./window")
target_link_libraries(Chip8 PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)