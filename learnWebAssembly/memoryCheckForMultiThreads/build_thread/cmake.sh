emcmake cmake \
 -G Ninja \
 -DCMAKE_BUILD_TYPE=Release \
 -DVTK_DIR=/work/vtk-960-build \
 -DBUILD_SHARED_LIBS=OFF \
 -DBUILD_THREAD_TYPE=Multi \
  ../
