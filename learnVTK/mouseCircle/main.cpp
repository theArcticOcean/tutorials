#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>

#include "CustomIteractorStyle.h"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

int main()
{
    vtkSPtrNew( cone, vtkConeSource );
    cone->Update();
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( cone->GetOutput() );

    vtkPolyData *data = cone->GetOutput();
    for( int i = 0; i < data->GetNumberOfPoints(); ++i )
    {
        double *pos = data->GetPoint( i );
        std::cout << "i: " << i << ", pos: " << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
    }


    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    vtkSPtrNew( iStyle, CustomIteractorStyle );
    iStyle->SetRenderer( renderer );
    renderWindowInteractor->SetInteractorStyle( iStyle );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
