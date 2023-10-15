file(GLOB hl_include_headers
        hashlink/src
        hashlink/include
        hashlink/include/pcre
)

file(GLOB pcre_srcs
        hashlink/include/pcre/pcre2_auto_possess.c
        hashlink/include/pcre/pcre2_chartables.c
        hashlink/include/pcre/pcre2_compile.c
        hashlink/include/pcre/pcre2_config.c
        hashlink/include/pcre/pcre2_context.c
        hashlink/include/pcre/pcre2_convert.c
        hashlink/include/pcre/pcre2_dfa_match.c
        hashlink/include/pcre/pcre2_error.c
        hashlink/include/pcre/pcre2_extuni.c
        hashlink/include/pcre/pcre2_find_bracket.c
        hashlink/include/pcre/pcre2_jit_compile.c
        hashlink/include/pcre/pcre2_maketables.c
        hashlink/include/pcre/pcre2_match_data.c
        hashlink/include/pcre/pcre2_match.c
        hashlink/include/pcre/pcre2_newline.c
        hashlink/include/pcre/pcre2_ord2utf.c
        hashlink/include/pcre/pcre2_pattern_info.c
        hashlink/include/pcre/pcre2_script_run.c
        hashlink/include/pcre/pcre2_serialize.c
        hashlink/include/pcre/pcre2_string_utils.c
        hashlink/include/pcre/pcre2_study.c
        hashlink/include/pcre/pcre2_substitute.c
        hashlink/include/pcre/pcre2_substring.c
        hashlink/include/pcre/pcre2_tables.c
        hashlink/include/pcre/pcre2_ucd.c
        hashlink/include/pcre/pcre2_valid_utf.c
        hashlink/include/pcre/pcre2_xclass.c
)

file(GLOB std_srcs
        hashlink/src/std/array.c
        hashlink/src/std/buffer.c
        hashlink/src/std/bytes.c
        hashlink/src/std/cast.c
        hashlink/src/std/date.c
        hashlink/src/std/debug.c
        hashlink/src/std/error.c
        hashlink/src/std/file.c
        hashlink/src/std/fun.c
        hashlink/src/std/maps.c
        hashlink/src/std/math.c
        hashlink/src/std/obj.c
        hashlink/src/std/random.c
        hashlink/src/std/regexp.c
        hashlink/src/std/socket.c
        hashlink/src/std/string.c
        hashlink/src/std/sys.c
        hashlink/src/std/track.c
        hashlink/src/std/types.c
        hashlink/src/std/ucs2.c
        hashlink/src/std/thread.c
        hashlink/src/std/process.c
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    list(APPEND std_srcs
            hashlink/include/mdbg/mdbg.c
            hashlink/include/mdbg/mach_excServer.c
            hashlink/include/mdbg/mach_excUser.c
    )
endif ()

if (ANDROID)
    list(APPEND std_srcs
            hashlink/src/std/sys_android.c
    )
endif ()

if (IOS_PLATFORM)
    list(APPEND std_srcs
            hashlink/src/std/sys_ios.m
    )
endif ()

# libhl
add_library(libhl SHARED
        ${pcre_srcs}
        ${std_srcs}
        hashlink/src/gc.c
)

target_include_directories(libhl PUBLIC
        hashlink/src
        hashlink/include
        hashlink/include/pcre
)

set(public_headers
        src/hl.h
        src/hlc.h
)

if(WIN32)
    set_target_properties(libhl
            PROPERTIES
            OUTPUT_NAME libhl
    )
else()
    set_target_properties(libhl
            PROPERTIES
            OUTPUT_NAME hl
    )
endif()

set_target_properties(libhl
        PROPERTIES
        VERSION "1.0"
        SOVERSION "1.0"
        PUBLIC_HEADER "${public_headers}"
        COMPILE_DEFINITIONS "_USRDLL;LIBHL_EXPORTS;HAVE_CONFIG_H;PCRE2_CODE_UNIT_WIDTH=16"
)
# libhl

add_library(hl
        hashlink/src/code.c
        hashlink/src/jit.c
        hashlink/src/module.c
        hashlink/src/debugger.c
        hashlink/src/profile.c
)


target_include_directories(hl PUBLIC
    ${hl_include_headers}
)

if (WIN32)
    target_link_libraries(libhl ws2_32 user32)
    target_link_libraries(hl user32)
else ()
    target_link_libraries(libhl m dl pthread)
endif ()

