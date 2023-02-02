#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOBJReader.h>
#include <vtkCamera.h>

#include "CustomIteractorStyle.h"

using namespace std;

int main()
{
    setbuf( stdout, nullptr );

    const string filePath = "8.obj";

    vtkSPtrNew( reader, vtkOBJReader );
    reader->SetFileName( filePath.c_str() );
    reader->Update();

    vtkSPtrNew( mapper1, vtkPolyDataMapper );
    mapper1->SetInputConnection( reader->GetOutputPort() );

    vtkSPtrNew( actor1, vtkActor );
    actor1->SetMapper( mapper1 );

    vtkSPtrNew( mapper2, vtkPolyDataMapper );
    mapper2->SetInputConnection( reader->GetOutputPort() );

    vtkSPtrNew( actor2, vtkActor );
    actor2->SetMapper( mapper2 );

    // Define viewport ranges
    // (xmin, ymin, xmax, ymax) left-top and right-bottom point
    double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
    double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};
    // Setup renderers
    vtkSmartPointer<vtkRenderer> leftRenderer =
        vtkSmartPointer<vtkRenderer>::New();
    leftRenderer->SetViewport( leftViewport );
    leftRenderer->SetBackground( .6, .5, .4 );
    leftRenderer->AddActor( actor1 );
    leftRenderer->ResetCamera();
    leftRenderer->GetActiveCamera()->ParallelProjectionOn();

    vtkSmartPointer<vtkRenderer> rightRenderer =
        vtkSmartPointer<vtkRenderer>::New();
    rightRenderer->SetViewport( rightViewport );
    rightRenderer->SetBackground( .4, .5, .6);
    rightRenderer->AddActor( actor2 );
    rightRenderer->ResetCamera();
    rightRenderer->GetActiveCamera()->ParallelProjectionOn();

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( leftRenderer );
    renderWindow->AddRenderer( rightRenderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    vtkSPtrNew( iStyle, CustomIteractorStyle );
    iStyle->Setm_LeftRender( leftRenderer );
    iStyle->Setm_RightRender( rightRenderer );
    iStyle->Init();

    renderWindowInteractor->SetInteractorStyle( iStyle );
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
