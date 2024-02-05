cmake_minimum_required(VERSION 3.24)
project(squirrel_embed)

set(CMAKE_CXX_STANDARD 20)

# Output every binary and libraries to bin folder
set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
endforeach ()

# Squirrel CMake gives us some options, here I'll be disabling some of them
# because we don't use them:
# SQ_DISABLE_INSTALLER: don't build scripts that installs Squirrel on our system.
# SQ_DISABLE_INTERPRETER: don't build the REPL.
# SQ_DISABLE_STATIC: Disable static libraries, we are opting for using only DLL's.
set(SQ_DISABLE_INSTALLER ON)
set(SQ_DISABLE_INTERPRETER ON)
set(SQ_DISABLE_STATIC ON)
add_subdirectory(squirrel)

add_executable(squirrel_embed main.cpp)
target_include_directories(squirrel_embed PUBLIC squirrel/include)
target_link_libraries(squirrel_embed squirrel sqstdlib)