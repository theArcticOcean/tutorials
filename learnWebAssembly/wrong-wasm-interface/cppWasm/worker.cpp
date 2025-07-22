#include "worker.h"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Worker::Worker()
{
    std::cout << "generate Worker!" << std::endl;

#ifdef __EMSCRIPTEN__
    //emscripten_run_script( "print()" ); // call JS function. it will open print window.
#endif
}

void Worker::ShowMyself(int value)
{
    std::cout << "I am a Worker: " << value << std::endl;
}

void Worker::OnInit()
{
#ifdef __EMSCRIPTEN__
    //emscripten_run_script("handleCreateWorkerAsync2();");
#endif
}