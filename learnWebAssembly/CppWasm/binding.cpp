#include <emscripten/bind.h>
#include "worker.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(worker) {
   class_<Worker>("Worker")
      .constructor()
      //.function("GetIntSum", &Worker::GetIntSum)
      ;
}