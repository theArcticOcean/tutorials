cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Release)
project(vtkWasmInWebWorker)

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
endif()

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(EMSCRIPTEN)
  set(emscripten_options)
  list(APPEND emscripten_options
    "-O3"
    "-g"
    #"-sNO_DISABLE_EXCEPTION_CATCHING" # allow catch expection from c++ wasm
  )

  set(emscripten_link_options)
  list(APPEND emscripten_link_options
  "--bind"
  #"-sNO_DISABLE_EXCEPTION_CATCHING" # allow catch expection from c++ wasm
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

target_compile_options(${PROJECT_NAME}
  PUBLIC
    ${emscripten_options}
)

target_link_options(${PROJECT_NAME}
  PUBLIC
    ${emscripten_link_options}
)

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )