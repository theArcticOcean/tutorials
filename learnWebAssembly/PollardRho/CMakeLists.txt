cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
project(Resolver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp resolver.cpp resolver.h)
