#include "worker.h"
#include "Log.hpp"
#include "CustomIteractorStyle.h"
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

    m_Picker = vtkSPtr<vtkCellPicker>::New();
    m_Picker->PickFromListOn();
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

    vtkSPtrNew( iStyle, CustomIteractorStyle );
    iStyle->Setm_Worder( this );
    m_RenderWindowInteractor->SetInteractorStyle( iStyle );

    m_Renderer->ResetCamera();
    m_RenderWindow->Render();

    m_Picker->AddPickList( m_Actor );

    Log( IInfo, "Init!" );
}

void Worker::Start()
{
    m_RenderWindowInteractor->Start();

    Log( IInfo, "Start!" );
}

void Worker::OnLeftButtonDown()
{
    auto eventPos = m_RenderWindowInteractor->GetEventPosition();
    m_Picker->Pick(eventPos[0], eventPos[1], 0, m_Renderer);
    if (m_Picker->GetPointId() > -1 )
    {
        vtkSPtr<vtkActor> actor = m_Picker->GetActor();
        Log( IInfo, "actor: ", actor );
    }
}