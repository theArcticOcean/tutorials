#include "worker.h"
#include "Log.hpp"
#include <cmath>
#include <chrono>
#include <unistd.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/threading.h>
#endif
#include <thread>
#include <future>
#include <atomic>

// C function for complex task in thread
double complex_task_thread_func(int len) {
#ifdef __EMSCRIPTEN__
    emscripten_run_script("console.log('Complex task started in std::thread web worker');");
#else
    Log(IInfo, "Complex task started in std::thread web worker");
#endif
    
    double result = 0.0;
    
    for (int i = 0; i < len; i++) {
        result += std::sqrt(i) * std::sin(i);
        for (int j = 0; j < len * 50; j++) {
            result += std::sqrt(j) * std::sin(j);
        }
        
        // Yield to other threads periodically
        if (i % 10 == 0) {
#ifdef __EMSCRIPTEN__
            emscripten_thread_sleep(0);
#else
            std::this_thread::yield();
#endif
        }
    }
    
#ifdef __EMSCRIPTEN__
    emscripten_run_script("console.log('Complex task completed in std::thread web worker');");
#else
    Log(IInfo, "Complex task completed in std::thread web worker");
#endif
    
    return result;
}

Worker::Worker()
{
#ifdef __EMSCRIPTEN__
    m_RenderWindowInteractor = vtkSPtr<vtkSDL2RenderWindowInteractor>::New();
    m_RenderWindow = vtkSPtr<vtkSDL2OpenGLRenderWindow>::New();
#else
    m_RenderWindowInteractor = vtkSPtr<vtkRenderWindowInteractor>::New();
    m_RenderWindow = vtkSPtr<vtkRenderWindow>::New();
#endif
}

void Worker::Init()
{
    vtkSPtrNew( cone, vtkConeSource );
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( cone->GetOutputPort() );

    m_Actor = vtkSPtr<vtkActor>::New();
    m_Actor->SetMapper( mapper );

    m_Renderer = vtkSPtr<vtkRenderer>::New();
    m_Renderer->SetBackground( 0, 0.2, 0 );
    m_Renderer->SetLayer( 0 );
    m_Renderer->AddActor(m_Actor);

    m_RenderWindow->AddRenderer( m_Renderer );
    m_RenderWindowInteractor->SetRenderWindow( m_RenderWindow );

    m_Renderer->ResetCamera();
    m_RenderWindow->Render();

    Log( IInfo, "Init!" );
}

void Worker::Start()
{
#ifdef __EMSCRIPTEN__
    emscripten_run_script( "console.log('hello world before start')" );
#endif
    m_RenderWindowInteractor->Start();

    Log( IInfo, "Start!" );
}

void Worker::SimuComplexTask()
{
    // Record start time
    auto start_time = std::chrono::high_resolution_clock::now();

    const int len = 3000;
    double result = 0.0;  // Use local variable with proper type
    for (int i = 0; i < len; i++) {
        result += std::sqrt(i) * std::sin(i);
        for (int j = 0; j < len*50; j++) {
            result += std::sqrt(j) * std::sin(j);
        }
    }
#ifdef __EMSCRIPTEN__
    emscripten_run_script( "console.log('SimuComplexTask hello world')" );
#endif
    Log(IInfo, "complex task completed with result: " + std::to_string(result));

    // Record end time and calculate duration
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    Log(IInfo, "complex task duration: " + std::to_string(duration.count()) + " ms");
}

int Worker::TestBind(int a, int b)
{
    int c = a + b;
    return c;
}

void Worker::SimuComplexTaskAsync()
{
    Log(IInfo, "Starting complex task in web worker thread using std::thread...");
    
    const int len = 3000;
    
    // Create a lambda function to wrap the task with timing and logging
    auto task_wrapper = [len]() {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        double result = complex_task_thread_func(len);
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        Log(IInfo, "Complex task completed with result: " + std::to_string(result));
        Log(IInfo, "Complex task duration: " + std::to_string(duration.count()) + " ms");
        Log(IInfo, "Complex task in web worker thread using std::thread finished!");
    };
    
    // Create and detach the thread to run asynchronously
    std::thread subThread(task_wrapper);
    subThread.detach();
    
    Log(IInfo, "Complex task thread started, main thread continues...");
}