#include "worker.h"
#include <iostream>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Worker::Worker()
{
    std::cout << "generate Worker!" << std::endl;

#ifdef __EMSCRIPTEN__
    //emscripten_run_script( "print()" ); // call JS function.
#endif
}

void Worker::ShowMyself()
{
    std::cout << "I am a Worker!" << std::endl;
}

void Worker::TryFunc()
{
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    int index = 10; // bad index.

    try {
        int value = numbers.at(index); 
        std::cout << "The value at index " << index << " is: " << value << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        std::cerr << "The requested index is out of the valid range for the vector." << std::endl;
    }
    // other exception
    catch (const std::exception& e) {
        std::cerr << "Caught a general exception: " << e.what() << std::endl;
    }

    std::cout << "Program continues to run after the try-catch block." << std::endl;
}