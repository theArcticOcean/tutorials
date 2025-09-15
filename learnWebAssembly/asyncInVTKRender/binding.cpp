#include <emscripten/bind.h>
#include "worker.h"

using namespace emscripten;

// Declare extern C functions
extern "C" {
    void worker_simu_complex_task_async();
    void worker_cleanup();
    void worker_start_rendering();
}

EMSCRIPTEN_BINDINGS(worker) {
   class_<Worker>("Worker")
      .constructor()
      .function("Init", &Worker::Init)
      .function("Start", &Worker::Start)
      .function("SimuComplexTask", &Worker::SimuComplexTask)
      .function("SimuComplexTaskAsync", &Worker::SimuComplexTaskAsync)
      .function("TestBind", &Worker::TestBind)
      ;
   
   // Export extern C functions
   function("worker_simu_complex_task_async", &worker_simu_complex_task_async);
   function("worker_cleanup", &worker_cleanup);
   function("worker_start_rendering", &worker_start_rendering);
}