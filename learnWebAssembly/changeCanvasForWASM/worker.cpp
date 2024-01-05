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

    m_Actor = vtkSPtr<vtkActor>::New();
    m_Actor->SetMapper( mapper );

    m_Renderer = vtkSPtr<vtkRenderer>::New();
    m_Renderer->AddActor(m_Actor);
    m_Renderer->SetBackground( 0, 0, 0 );

    m_RenderWindow->AddRenderer( m_Renderer );
    m_RenderWindowInteractor->SetRenderWindow( m_RenderWindow );

    m_Renderer->ResetCamera();
    m_RenderWindow->Render();

    std::cout << "Init!\n";
}

/**
 * @brief   the interface is used in web page
 * 
 * @return  nothing
 */
void Worker::ChangeRenderWindow()
{
    //m_RenderWindowInteractor->TerminateApp();
    
    std::cout << "m_Renderer: " << m_Renderer << std::endl;
    std::cout << "m_RenderWindowInteractor: " << m_RenderWindowInteractor << std::endl;

    m_Renderer = vtkSPtr<vtkRenderer>::New(); // need it!
    m_Renderer->AddActor(m_Actor);
    m_Renderer->SetBackground( 0, 0, 0 );

    m_RenderWindow = vtkSPtr<vtkSDL2OpenGLRenderWindow>::New(); // need
    m_RenderWindow->AddRenderer( m_Renderer );
    //m_RenderWindowInteractor = vtkSPtr<vtkSDL2RenderWindowInteractor>::New();
    m_RenderWindowInteractor->SetRenderWindow( m_RenderWindow );

    m_Renderer->ResetCamera();
    m_RenderWindow->Render();
    m_RenderWindowInteractor->Start();
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
