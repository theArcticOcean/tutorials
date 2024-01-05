#include <emscripten/bind.h>
#include "worker.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(worker) {
   class_<Worker>("Worker")
      .constructor()
      .function("Init", &Worker::Init)
      .function("Start", &Worker::Start)
      .function("ReStart", &Worker::ReStart)
      .function("Terminate", &Worker::Terminate)
      .function("ChangeRenderWindow", &Worker::ChangeRenderWindow)
      ;
}