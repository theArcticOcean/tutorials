cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Release)
project(passArrayToCPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( Hdrs passArrayToCPP.h )
set( Srcs passArrayToCPP.cpp )

if(EMSCRIPTEN)
  list(APPEND Srcs
    binding.cpp
  )
else()
  list(APPEND Srcs
    #main.cpp
  )
endif()


if(EMSCRIPTEN)
  set(emscripten_options)
  list(APPEND emscripten_options
    "--bind"
    "-O3"
    "-g"
    "-DLOG_ON"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=['allocate,stringToUTF8,UTF8ToString,intArrayFromString']"
    "SHELL:-s -fdebug-compilation-dir=."
    "SHELL:-s EXPORT_NAME=createModule"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s DEMANGLE_SUPPORT=1"
    "SHELL:-s EMULATE_FUNCTION_POINTER_CASTS=0"
    "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
    "SHELL:-s MODULARIZE=1"
    "SHELL:-s WASM=1"
    "SHELL:-s --no-heap-copy"
    "SHELL:-s INITIAL_MEMORY=200MB"
    "SHELL:-s MAXIMUM_MEMORY=4000MB"
    "SHELL:-s ASSERTIONS=2"
  )
endif()

add_executable(${PROJECT_NAME} ${Hdrs} ${Srcs})

target_compile_options(${PROJECT_NAME}
  PUBLIC
    ${emscripten_options}
)

target_link_options(${PROJECT_NAME}
  PUBLIC
    ${emscripten_options}
)