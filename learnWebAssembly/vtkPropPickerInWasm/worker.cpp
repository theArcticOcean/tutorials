#include "worker.h"
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

    m_Picker = vtkSPtr<vtkPropPicker>::New();
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

    std::cout << "Init!\n";
}

void Worker::Start()
{
    m_RenderWindowInteractor->Start();

    std::cout << "Start!\n";
}

void Worker::OnLeftButtonDown()
{
    std::cout << "OnLeftButtonDown!\n";
    auto eventPos = m_RenderWindowInteractor->GetEventPosition();
    int ret = m_Picker->Pick( eventPos[0], eventPos[1], 0, m_Renderer );
    std::cout << "Pick: " << ret << std::endl;
    ret = m_Picker->PickProp( eventPos[0], eventPos[1], m_Renderer );
    std::cout << "PickProp: " << ret << std::endl;
    if( ret != 0 )
    {
        std::cout << "picked!\n";
    }
}