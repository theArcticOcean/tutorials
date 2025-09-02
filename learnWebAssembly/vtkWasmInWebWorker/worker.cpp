#include "worker.h"
#include "Log.hpp"
#include <cmath>
#include <chrono>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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
    Log(IInfo, "complex task completed with result: " + std::to_string(result));

    // Record end time and calculate duration
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    Log(IInfo, "complex task duration: " + std::to_string(duration.count()) + " ms");
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
    
    Log(IInfo, "Async complex task completed with result: " + std::to_string(result));

    // Record end time and calculate duration
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    Log(IInfo, "Async complex task duration: " + std::to_string(duration.count()) + " ms");

#endif
}