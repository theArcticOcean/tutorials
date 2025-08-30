#include "worker.h"
#include "Log.hpp"

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

void Worker::OnLeftButtonDown()
{
}