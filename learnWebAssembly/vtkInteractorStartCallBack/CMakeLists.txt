project(vtkInteractorStartCallBack)

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
  set(emscripten_options)
  list(APPEND emscripten_options
    "--bind"
    #"-O3"
    #"-g"
    "-DLOG_OFF"
    "SHELL:-s USE_SDL=2"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=['allocate,stringToUTF8,UTF8ToString,FS,intArrayFromString']"
    "SHELL:-s -fdebug-compilation-dir=."
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
    message("Configuring data directory for wasm..........")
endif()

set( Srcs 
    worker.h
    worker.cpp
    UInteractor.h
    UInteractor.cpp
    )

if(EMSCRIPTEN)
list(APPEND Srcs
    binding.cpp
)
else()
list(APPEND Srcs
    main.cpp
)
endif()

add_executable(${PROJECT_NAME} ${Srcs})

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
    ${emscripten_options}
)

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} ${LINK_FLAGS} )