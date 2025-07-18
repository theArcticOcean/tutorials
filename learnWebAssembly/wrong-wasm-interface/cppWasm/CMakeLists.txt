cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Release)
project(cppWasm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( Hdrs worker.h )
set( Srcs worker.cpp )

if(EMSCRIPTEN)
  list(APPEND Srcs
    binding.cpp
  )
endif()


if(EMSCRIPTEN)
  set(emscripten_options)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND emscripten_options "-O3")
  endif()

  set(emscripten_link_options)
  # list(APPEND emscripten_link_options
  #   "--bind"
  #   #"-O3"
  #   "-g"
  #   "SHELL:-s EXPORTED_RUNTIME_METHODS=['allocate,stringToUTF8,UTF8ToString,intArrayFromString']"
  #   "SHELL:-s -fdebug-compilation-dir=."
  #   "SHELL:-s EXPORT_NAME=createModule"
  #   "SHELL:-s ALLOW_MEMORY_GROWTH=1"
  #   "SHELL:-s DEMANGLE_SUPPORT=1"
  #   "SHELL:-s EMULATE_FUNCTION_POINTER_CASTS=0"
  #   "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
  #   "SHELL:-s MODULARIZE=1"
  #   "SHELL:-s WASM=1"
  #   "SHELL:-s --no-heap-copy"
  #   "SHELL:-s INITIAL_MEMORY=200MB"
  #   "SHELL:-s MAXIMUM_MEMORY=4000MB"
  #   "SHELL:-s ASSERTIONS=2"
  # )

  # list(APPEND emscripten_link_options
  # #list(APPEND emscripten_options
  # "SHELL:-s ASSERTIONS=2"                    # Controls runtime assertions
  # "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"    # Enables exception catching
  # "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"    # Allows undefined symbols
  # "SHELL:-s DEMANGLE_SUPPORT=1"              # Enables symbol demangling
  # )

  list(APPEND emscripten_link_options
  "--bind"
  "SHELL:-s EXPORT_NAME=createModule"
  "SHELL:-s USE_SDL=2"
  "SHELL:-s EXPORTED_RUNTIME_METHODS=['allocateUTF8, allocate,stringToUTF8,UTF8ToString,FS,intArrayFromString']"
  "SHELL:-s EXTRA_EXPORTED_RUNTIME_METHODS=['GL']"
  "SHELL:-s ALLOW_MEMORY_GROWTH=1"
  "SHELL:-s DEMANGLE_SUPPORT=1"
  "SHELL:-s EMULATE_FUNCTION_POINTER_CASTS=0"
  "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
  "SHELL:-s STACK_SIZE=5MB" # After emsdk 3.1.27, the default stack size is only 64KB and too small 
  "SHELL:-s MODULARIZE=1"
  "SHELL:-s WASM=1"
  "SHELL:-s ENVIRONMENT=web,worker"
  "SHELL:-s USE_WEBGL2=1" # for safari must use webgl=1.1, so set false here
  "SHELL:-s FULL_ES2=1"
  "SHELL:-s FULL_ES3=1"
  "SHELL:-s --no-heap-copy"
  "SHELL:-s INITIAL_MEMORY=150MB"
  "SHELL:-s MAXIMUM_MEMORY=4096MB"
  "SHELL:-s ASSERTIONS=2"
  "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
  # "SHELL:-s -Werror"  # warn as errors to be emitted
  "SHELL:-s TEXTDECODER=0"
  )
endif()

add_executable(${PROJECT_NAME} ${Hdrs} ${Srcs})

target_compile_options(${PROJECT_NAME}
  PUBLIC
    ${emscripten_options}
)

target_link_options(${PROJECT_NAME}
  PUBLIC
    ${emscripten_link_options}
)