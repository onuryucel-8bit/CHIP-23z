cmake_minimum_required(VERSION 3.5)

project(CHIP-8 VERSION 0.0.1)

set(SOURCES_CHIP_8 "./chip8/Chip8.cpp" "./chip8/romLoaderCH8.cpp")
set(SOURCES_UTILS "./utils/Radix.cpp")
set(SOURCES_IO "./window/Window_SFML.cpp")

set(SFML_DIR ./SFML/lib/cmake/SFML)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

file(COPY
    "${CMAKE_SOURCE_DIR}/res/Retro Gaming.ttf"
    DESTINATION "${CMAKE_BINARY_DIR}/Debug/res"
)

file(COPY
    "${CMAKE_SOURCE_DIR}/res/ch8_squareWave.wav"
    DESTINATION "${CMAKE_BINARY_DIR}/Debug/res"
)

#read doc for info
    file(GLOB_RECURSE ROM_FILES"${CMAKE_SOURCE_DIR}/ROMs/*.mch8")

    foreach(TXT_FILE ${ROM_FILES})
        get_filename_component(FILE_NAME ${ROM_FILES} NAME)
        file(COPY ${ROM_FILES} DESTINATION "${CMAKE_BINARY_DIR}/Debug/ROMs/")
    endforeach()


add_executable(Chip8 main.cpp ${SOURCES_IO}  ${SOURCES_CHIP_8} ${SOURCES_UTILS} )

include_directories("./chip8" "./utils" "./window")
target_link_libraries(Chip8 PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)