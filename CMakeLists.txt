cmake_policy(SET CMP0048 NEW)
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

cmake_minimum_required(VERSION 3.0.0)
project(imdb_bot VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(Boost_USE_MULTITHREADED ON)

add_executable(imdb_bot main.cpp)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL)
find_package(Boost COMPONENTS system REQUIRED)

include_directories(
    /usr/local/include 
    ${OPENSSL_INCLUDE_DIR} 
    ${Boost_INCLUDE_DIR}
)

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

target_include_directories(imdb_bot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(imdb_bot
    /usr/local/lib/libTgBot.a
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OPENSSL_LIBRARIES} 
    ${CURL_LIBRARIES}
    ${Boost_LIBRARIES} 
)

