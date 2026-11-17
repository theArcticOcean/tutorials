cmake_minimum_required(VERSION 2.8)

project(FastMarchingGeodesicDistance)

find_package( VTK REQUIRED )

set( FastMarching
  algorithm/gw_core/GW_Config.cpp
  algorithm/gw_core/GW_Face.cpp
  algorithm/gw_core/GW_FaceIterator.cpp
  algorithm/gw_core/GW_Mesh.cpp
  algorithm/gw_core/GW_SmartCounter.cpp
  algorithm/gw_core/GW_Vertex.cpp
  algorithm/gw_core/GW_VertexIterator.cpp
  algorithm/gw_geodesic/GW_GeodesicFace.cpp
  algorithm/gw_geodesic/GW_GeodesicMesh.cpp
  algorithm/gw_geodesic/GW_GeodesicPath.cpp
  algorithm/gw_geodesic/GW_GeodesicPoint.cpp
  algorithm/gw_geodesic/GW_GeodesicVertex.cpp
  algorithm/gw_geodesic/GW_TriangularInterpolation_Linear.cpp
  algorithm/gw_geodesic/GW_TriangularInterpolation_Quadratic.cpp
  algorithm/gw_geodesic/GW_TriangularInterpolation_Cubic.cpp

  algorithm/gw_core/GW_Config.h
  algorithm/gw_core/GW_Face.h
  algorithm/gw_core/GW_Mesh.h
  algorithm/gw_core/GW_FaceIterator.h
  algorithm/gw_core/GW_Vertex.h
  algorithm/gw_core/GW_VertexIterator.h
  algorithm/gw_core/GW_SmartCounter.h
  algorithm/gw_geodesic/GW_GeodesicFace.h
  algorithm/gw_geodesic/GW_GeodesicMesh.h
  algorithm/gw_geodesic/GW_GeodesicPath.h
  algorithm/gw_geodesic/GW_GeodesicPoint.h
  algorithm/gw_geodesic/GW_GeodesicVertex.h
  algorithm/gw_geodesic/GW_TriangularInterpolation_Linear.h
  algorithm/gw_geodesic/GW_TriangularInterpolation_Quadratic.h
  algorithm/gw_geodesic/GW_TriangularInterpolation_Cubic.h

  algorithm/vtkFastMarchingGeodesicDistance.h
  algorithm/vtkFastMarchingGeodesicDistance.cxx
  algorithm/vtkPolyDataGeodesicDistance.h
  algorithm/vtkPolyDataGeodesicDistance.cxx
)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  algorithm/gw_core
  algorithm/gw_geodesic
  algorithm/gw_geodesic/ls
  algorithm/
)

source_group(FastMarching FILES ${FastMarching})

add_executable(${PROJECT_NAME}
    "main.cpp"
    ${FastMarching}
    )

if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
else ()
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
    )
endif ()

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )