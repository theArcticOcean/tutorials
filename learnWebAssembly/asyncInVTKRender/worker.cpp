#include "worker.h"
#include "Log.hpp"
#include <cmath>
#include <chrono>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Extern C wrapper functions to avoid C++ name mangling
extern "C" {
    // Global worker instance
    static Worker* g_worker = nullptr;
    
    // Export SimuComplexTaskAsync function
    void worker_simu_complex_task_async() {
        if (g_worker) {
            g_worker->SimuComplexTaskAsync();
        }
    }
    
    // Cleanup worker instance
    void worker_cleanup() {
        if (g_worker) {
            delete g_worker;
            g_worker = nullptr;
        }
    }

    void worker_start_rendering() {
        if (g_worker) {
            g_worker->Start();
        }
    }
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

    g_worker = this;
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
    emscripten_run_script( "console.log('hello world before start')" );
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
    emscripten_run_script( "console.log('SimuComplexTask hello world')" );
    Log(IInfo, "complex task completed with result: " + std::to_string(result));

    // Record end time and calculate duration
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    Log(IInfo, "complex task duration: " + std::to_string(duration.count()) + " ms");
}

int Worker::TestBind(int a, int b)
{
    int c = a + b;
    emscripten_run_script( "addWasmCallbackInterface();" );
    return c;
}

void Worker::SimuComplexTaskAsync()
{
#ifdef __EMSCRIPTEN__
    // Record start time
    auto start_time = std::chrono::high_resolution_clock::now();

    const int len = 3000;
    double result = 0.0;
    
    for (int i = 0; i < len; i++) {
        result += std::sqrt(i) * std::sin(i);
        for (int j = 0; j < len*50; j++) {
            result += std::sqrt(j) * std::sin(j);
        }
        
        // Yield to the event loop every 10 iterations to prevent blocking
        if (i % 10 == 0) {
            emscripten_sleep(0);
        }
    }
    emscripten_run_script( "console.log('SimuComplexTaskAsync hello world')" );
    Log(IInfo, "Async complex task completed with result: " + std::to_string(result));

    // Record end time and calculate duration
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    Log(IInfo, "Async complex task duration: " + std::to_string(duration.count()) + " ms");

#endif
}