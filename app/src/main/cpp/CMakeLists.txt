
cmake_minimum_required(VERSION 3.10.2)

project("jnigifplayer")

add_library(
        jnigifplayer

        SHARED

        native-lib.cpp)

find_library(
        log-lib

        log)

target_link_libraries(
        jnigifplayer

        ${log-lib})