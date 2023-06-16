#include "worker.h"
#include <emscripten.h>

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

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    m_RenderWindow->AddRenderer( renderer );
    m_RenderWindowInteractor->SetRenderWindow( m_RenderWindow );

    renderer->ResetCamera();
    m_RenderWindow->Render();

    std::cout << "Init!\n";
}

void Worker::Start()
{
    m_RenderWindowInteractor->Start();

    std::cout << "Start!\n";
}

void Worker::Terminate()
{
    m_RenderWindowInteractor->TerminateApp(); // same to emscripten_cancel_main_loop
}

void Worker::ReStart()
{
    m_RenderWindowInteractor->ReInitialize();
    m_RenderWindowInteractor->Start();
}
