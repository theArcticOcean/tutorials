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
  # 编译选项
  set(emscripten_compile_options)
  list(APPEND emscripten_compile_options
    "--bind"
    "-O3"
    "-g"
    "-DLOG_OFF"
    "-fdebug-compilation-dir=."
  )
  
  # 链接选项
  set(emscripten_link_options)
  list(APPEND emscripten_link_options
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
    "SHELL:-s --no-heap-copy"
    "SHELL:-s INITIAL_MEMORY=200MB"
    "SHELL:-s MAXIMUM_MEMORY=512MB"
    "SHELL:-s ASSERTIONS=2"
    "SHELL:-s TOTAL_MEMORY=512MB"
    "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
  )
  
  # 检查是否需要多线程支持
  if(BUILD_THREAD_TYPE STREQUAL "Multi")
    message(STATUS "Configuring build with pthread support")
    # 多线程编译选项
    list(APPEND emscripten_compile_options
      "-pthread"
    )
    # 多线程链接选项
    list(APPEND emscripten_link_options
      "-pthread"
      "SHELL:-s USE_PTHREADS=1"
      "SHELL:-s PTHREAD_POOL_SIZE=4"
      "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    )
    # 添加多线程相关的宏定义
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
