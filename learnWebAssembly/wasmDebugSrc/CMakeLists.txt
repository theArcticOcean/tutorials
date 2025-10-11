project(vtkWasmTest)

if (VTK_VERSION VERSION_LESS "8.9") #"9.0.3"
    find_package( VTK REQUIRED )
else ()
    find_package( VTK COMPONENTS vtkCommonCore vtkRenderingCore
        vtkInteractionStyle vtkRenderingOpenGL2 vtkFiltersSources
        vtkRenderingFreeType IOGeometry )
endif()

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(EMSCRIPTEN)
  # Set default build type if not specified
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
  endif()
  
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
  
  # Common compile options for both Debug and Release
  set(emscripten_compile_options_common)
  list(APPEND emscripten_compile_options_common
    "--bind"
  )
  
  # Common link options for both Debug and Release
  set(emscripten_link_options_common)
  list(APPEND emscripten_link_options_common
    "--bind"
    "SHELL:-s USE_SDL=2"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=['allocate,stringToUTF8,UTF8ToString,FS,intArrayFromString']"
    "SHELL:-s EXTRA_EXPORTED_RUNTIME_METHODS=['ALLOC_NORMAL']"
    "SHELL:-s EXPORT_NAME=tester"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s DEMANGLE_SUPPORT=1"
    "SHELL:-s EMULATE_FUNCTION_POINTER_CASTS=0"
    "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
    "SHELL:-s MODULARIZE=1"
    "SHELL:-s WASM=1"
    "SHELL:-s INITIAL_MEMORY=200MB"
    "SHELL:-s MAXIMUM_MEMORY=512MB"
    "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
  )
  
  # Debug-specific options - 针对emsdk 3.1.51优化
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring Debug build for emsdk 3.1.51+")
    
    set(emscripten_compile_options_debug)
    list(APPEND emscripten_compile_options_debug
      "-O0"                          # No optimization
      "-g3"                          # 改为g3，3.1.51版本更兼容
    )
    
    set(emscripten_link_options_debug)
    list(APPEND emscripten_link_options_debug
      "-g3"
    )
    
    # Combine common and debug options
    set(emscripten_compile_options ${emscripten_compile_options_common} ${emscripten_compile_options_debug})
    set(emscripten_link_options ${emscripten_link_options_common} ${emscripten_link_options_debug})
    
  # Release-specific options
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring Release build with optimizations")
    
    set(emscripten_compile_options_release)
    list(APPEND emscripten_compile_options_release
      "-O3"
    )
    
    set(emscripten_link_options_release)
    list(APPEND emscripten_link_options_release
      "SHELL:-s --no-heap-copy"
    )
    
    set(emscripten_compile_options ${emscripten_compile_options_common} ${emscripten_compile_options_release})
    set(emscripten_link_options ${emscripten_link_options_common} ${emscripten_link_options_release})
    
  else()
    message(WARNING "Unknown build type: ${CMAKE_BUILD_TYPE}, using Debug settings")
    set(CMAKE_BUILD_TYPE "Debug")
  endif()
  
  # 检查是否需要多线程支持
  if(BUILD_THREAD_TYPE STREQUAL "Multi")
    message(STATUS "Configuring build with pthread support")
    list(APPEND emscripten_compile_options
      "-pthread"
    )
    list(APPEND emscripten_link_options
      "-pthread"
      "-sUSE_PTHREADS=1"
      "-sPTHREAD_POOL_SIZE=4"
      "-sALLOW_MEMORY_GROWTH=1"
    )
    add_definitions(-DUSE_PTHREADS)
  else()
    message(STATUS "Configuring regular build without pthread support")
  endif()
  
  message("Configuring data directory for wasm..........")
endif()

set( cpps 
    main.cpp
    )

add_executable(${PROJECT_NAME} ${cpps})

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
    ${emscripten_compile_options}
)

target_link_options(${PROJECT_NAME}
  PUBLIC
    ${emscripten_link_options}
)

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} ${LINK_FLAGS} )
