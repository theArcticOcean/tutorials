#include <emscripten/bind.h>
#include "passArrayToCPP.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(worker) {
   class_<Worker>("Worker")
      .constructor()
      .function("GetIntGum", &Worker::GetIntGum)
      ;
}