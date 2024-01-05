#include "worker.h"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Worker::Worker()
{
    std::cout << "generate Worker!" << std::endl;

#ifdef __EMSCRIPTEN__
    emscripten_run_script( "print()" ); // call JS function.
#endif
}