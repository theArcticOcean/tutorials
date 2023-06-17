#include <iostream>
#include <stdio.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>

#include "CustomIteractorStyle.h"

#ifdef __EMSCRIPTEN__
#include "vtkSDL2OpenGLRenderWindow.h"
#include "vtkSDL2RenderWindowInteractor.h"
#include <unistd.h>
#endif // __EMSCRIPTEN__

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

// export the function.
extern "C"
{
    void setup()
    {
        printf( "let's start!\n" );
    }

    int compute( int a, int b )
    {
        int ans = 1;

        for( int i = 0; i < b; ++i )
        {
            sleep(1);
            ans = ans * a;
        }
        return ans;
    }
}

int main()
{
    vtkSPtrNew( cone, vtkConeSource );
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( cone->GetOutputPort() );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

#ifdef __EMSCRIPTEN__
    vtkSPtrNew(renderWindow, vtkSDL2OpenGLRenderWindow);
#else
    vtkSPtrNew(renderWindow, vtkRenderWindow);
#endif
    renderWindow->AddRenderer( renderer );

#ifdef __EMSCRIPTEN__
    vtkSPtrNew(renderWindowInteractor, vtkSDL2RenderWindowInteractor);
#else
    vtkSPtrNew(renderWindowInteractor, vtkRenderWindowInteractor);
#endif

    renderWindowInteractor->SetRenderWindow( renderWindow );
    renderer->ResetCamera();
        
    vtkSPtrNew( iStyle, CustomIteractorStyle );
    renderWindowInteractor->SetInteractorStyle( iStyle );

    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}