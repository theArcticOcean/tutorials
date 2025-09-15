#!/bin/bash

# Default is Release, use 'debug' as first argument for Debug build
if [ "$1" = "debug" ]; then
    BUILD_TYPE="Debug"
else
    BUILD_TYPE="Release"
fi

echo "Building with CMAKE_BUILD_TYPE=$BUILD_TYPE"
cmake -DVTK_DIR=/work/vtk910-mthreads-build -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../