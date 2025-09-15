#include "worker.h"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Starting VTK Worker Desktop Application..." << std::endl;
    
    // Create worker instance
    Worker worker;
    
    // Initialize the worker
    worker.Init();
    
    worker.SimuComplexTaskAsync();

    // Start the worker (this will start the VTK render loop)
    worker.Start();
    
    std::cout << "Application finished." << std::endl;
    return 0;
}