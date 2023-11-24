cmake_minimum_required(VERSION 3.1)
project(native_extensions)

set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

function(set_as_hdll target)
    set_target_properties(${target}.hdll
            PROPERTIES
            PREFIX ""
            OUTPUT_NAME ${target}
            SUFFIX .hdll
    )
endfunction()

add_library(ring_buffer.hdll SHARED lib/ring_buffer.c)
set_as_hdll(ring_buffer)
target_include_directories(ring_buffer.hdll PUBLIC external/hashlink/include)
target_link_directories(ring_buffer.hdll PUBLIC external/hashlink)
target_link_libraries(ring_buffer.hdll PUBLIC libhl)
