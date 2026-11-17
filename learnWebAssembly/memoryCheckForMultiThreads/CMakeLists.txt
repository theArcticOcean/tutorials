cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)
project(memoryCheckForMultiThreads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (VTK_VERSION VERSION_LESS "8.9") #"9.0.3"
    find_package( VTK REQUIRED )
else ()
    find_package( VTK COMPONENTS vtkCommonCore vtkRenderingCore
        vtkInteractionStyle vtkRenderingOpenGL2 vtkFiltersSources
        vtkRenderingFreeType IOGeometry )
endif()

set( Hdrs Log.hpp worker.h )
set( Srcs worker.cpp )

if(EMSCRIPTEN)
  list(APPEND Srcs
    binding.cpp
  )
else()
  # Desktop version - add main.cpp
  list(APPEND Srcs
    main.cpp
  )
endif()

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(EMSCRIPTEN)
  set(emscripten_options)
  list(APPEND emscripten_options
    "-pthread"
    #"-sNO_DISABLE_EXCEPTION_CATCHING" # allow catch expection from c++ wasm
  )

  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND emscripten_options "-O3") 
  elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND emscripten_options "-O0") 
    list(APPEND emscripten_options "-g3") 
  endif()

  set(emscripten_link_options)
  list(APPEND emscripten_link_options
  "--bind"
  #"-pthread"
  "SHELL:-s USE_SDL=2"
  "SHELL:-s ENVIRONMENT=web,worker"
  "SHELL:-s STACK_SIZE=5MB"
  "SHELL:-s USE_WEBGL2=1" # for safari must use webgl=1.1, so set false here
  "SHELL:-s FULL_ES2=1"
  "SHELL:-s FULL_ES3=1"
  #"-sNO_DISABLE_EXCEPTION_CATCHING" # allow catch expection from c++ wasm
  "SHELL:-s EXPORTED_RUNTIME_METHODS=['allocate,stringToUTF8,UTF8ToString,intArrayFromString, GL']" #,'Asyncify'
  "SHELL:-s -fdebug-compilation-dir=."
  "SHELL:-s EXPORT_NAME=createModule"
  "SHELL:-s ALLOW_MEMORY_GROWTH=1"
  #"SHELL:-s DEMANGLE_SUPPORT=1"
  "SHELL:-s EMULATE_FUNCTION_POINTER_CASTS=0"
  "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
  "SHELL:-s MODULARIZE=1"
  "SHELL:-s WASM=1"
  "SHELL:-s --no-heap-copy"
  "SHELL:-s INITIAL_MEMORY=200MB"
  "SHELL:-s MAXIMUM_MEMORY=4000MB"
  "SHELL:-s TEXTDECODER=1"
  "SHELL:-s ASSERTIONS=2"
  "SHELL:-s USE_PTHREADS=1"
  "SHELL:-s PTHREAD_POOL_SIZE=navigator.hardwareConcurrency"
  "SHELL:-s PTHREAD_POOL_SIZE_STRICT=0"
  # Don't block main() while every pooled Worker instantiates the module.
  "SHELL:-s PTHREAD_POOL_DELAY_LOAD=1"
  # Default dlmalloc has one global lock — allocation-heavy VTK code (DeepCopy,
  # locators) serializes across threads on it, eating the fan-out speedup.
  # mimalloc keeps per-thread heaps.
  "SHELL:-s MALLOC=mimalloc"
  )
endif()

add_executable(${PROJECT_NAME} ${Hdrs} ${Srcs})

if (VTK_VERSION VERSION_LESS "8.9")
  # old system
  include(${VTK_USE_FILE})
else ()
  # vtk_module_autoinit is needed
  message("autoinit module for higher version vtk")
  vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
    )
endif ()

if(EMSCRIPTEN)
  target_compile_options(${PROJECT_NAME}
    PUBLIC
      ${emscripten_options}
  )

  target_link_options(${PROJECT_NAME}
    PUBLIC
      ${emscripten_link_options}
  )
else()
  # Desktop version compile options
  find_package(Threads REQUIRED)
  target_link_libraries(${PROJECT_NAME} Threads::Threads)
  
  # Enable C++11 threading support
  target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
  
  # Desktop debug/release compile options
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
  endif()
endif()

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )