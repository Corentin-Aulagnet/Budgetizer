cmake_minimum_required(VERSION 3.5)
project(sqlite3 VERSION 1.0)
set(CMAKE_BUILD_TYPE Release)

include_directories(sqlite3)
add_library(sqlite3 STATIC sqlite3.c)



