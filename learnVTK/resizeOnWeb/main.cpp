#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindowInteractor.h>

#ifdef __EMSCRIPTEN__
#include "vtkSDL2OpenGLRenderWindow.h"
#include "vtkSDL2RenderWindowInteractor.h"
#include <emscripten/emscripten.h>
#endif // __EMSCRIPTEN__


#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

void CallbackFunction(vtkObject* caller,
                        long unsigned int vtkNotUsed(eventId),
                        void* vtkNotUsed(clientData),
                        void* vtkNotUsed(callData))
{
    std::cout << "rezie: " << std::endl;
}

int main()
{
    setbuf(stdout,NULL);
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

    vtkSmartPointer<vtkCallbackCommand> cbObj = vtkSmartPointer<vtkCallbackCommand>::New();
    cbObj->SetCallback( CallbackFunction );
    renderWindow->AddObserver(vtkCommand::WindowResizeEvent, cbObj);

#ifdef __EMSCRIPTEN__
    vtkSPtrNew(renderWindowInteractor, vtkSDL2RenderWindowInteractor);

    EM_ASM({
        window.addEventListener("resize", function() { // it make sure VTK listen for it
            console.log( "resize event!" );
        });
    });
#else
    vtkSPtrNew(renderWindowInteractor, vtkRenderWindowInteractor);
#endif
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    std::cout << "start!" << std::endl;
    renderWindowInteractor->Start();
    return 0;
}