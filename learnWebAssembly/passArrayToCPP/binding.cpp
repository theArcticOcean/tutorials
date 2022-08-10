#include <emscripten/bind.h>
#include "passArrayToCPP.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(worker) {
   class_<Worker>("Worker")
      .constructor()
      .function("GetIntSum", &Worker::GetIntSum)
      .function("GetDoubleSum", &Worker::GetDoubleSum)
      .function("ShowBoolArray", &Worker::ShowBoolArray)
      .function("GetIntArray", &Worker::GetIntArray)
      .function("GetFloatArray", &Worker::GetFloatArray)
      ;
}