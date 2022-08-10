cmake_policy(SET CMP0048 NEW)
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

cmake_minimum_required(VERSION 3.0.0)
project(imdb_bot VERSION 0.1.0)

include(FindPkgConfig)
pkg_check_modules(CURLPP REQUIRED curlpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(Boost_USE_MULTITHREADED ON)

add_executable(imdb_bot main.cpp)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL)
find_package(Boost COMPONENTS system REQUIRED)

set(GUMBO_INCLUDE_DIR /home/dan/gumbo-parser/src/)

set(
    GUMBO_SOURCES
    ${GUMBO_INCLUDE_DIR}attribute.h
    ${GUMBO_INCLUDE_DIR}attribute.c
    ${GUMBO_INCLUDE_DIR}char_ref.h
    ${GUMBO_INCLUDE_DIR}char_ref.c
    ${GUMBO_INCLUDE_DIR}error.h
    ${GUMBO_INCLUDE_DIR}error.c
    ${GUMBO_INCLUDE_DIR}gumbo.h
    ${GUMBO_INCLUDE_DIR}insertion_mode.h
    ${GUMBO_INCLUDE_DIR}parser.h
    ${GUMBO_INCLUDE_DIR}parser.c
    ${GUMBO_INCLUDE_DIR}string_buffer.h
    ${GUMBO_INCLUDE_DIR}string_buffer.c
    ${GUMBO_INCLUDE_DIR}string_piece.h
    ${GUMBO_INCLUDE_DIR}string_piece.c
    ${GUMBO_INCLUDE_DIR}tag_enum.h
    ${GUMBO_INCLUDE_DIR}tag_gperf.h
    ${GUMBO_INCLUDE_DIR}tag_sizes.h
    ${GUMBO_INCLUDE_DIR}tag_strings.h
    ${GUMBO_INCLUDE_DIR}tag.c
    ${GUMBO_INCLUDE_DIR}token_type.h
    ${GUMBO_INCLUDE_DIR}tokenizer.h
    ${GUMBO_INCLUDE_DIR}tokenizer.c
    ${GUMBO_INCLUDE_DIR}tokenizer_states.h
    ${GUMBO_INCLUDE_DIR}utf8.h
    ${GUMBO_INCLUDE_DIR}utf8.c
    ${GUMBO_INCLUDE_DIR}util.h
    ${GUMBO_INCLUDE_DIR}util.c
    ${GUMBO_INCLUDE_DIR}vector.h
    ${GUMBO_INCLUDE_DIR}vector.c
)

add_library(gumbo STATIC ${GUMBO_SOURCES})

include_directories(
    /usr/local/include 
    ${OPENSSL_INCLUDE_DIR} 
    ${Boost_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
    ${GUMBO_INCLUDE_DIR}
)

add_definitions(-DHAVE_CURL)

target_include_directories(imdb_bot PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    imdb_bot
    /usr/local/lib/libTgBot.a
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OPENSSL_LIBRARIES} 
    ${CURL_LIBRARIES}
    ${Boost_LIBRARIES} 
    ${CURLPP_LDFLAGS}
    /home/dan/curlpp/build/libcurlpp.so.1
    gumbo
)
