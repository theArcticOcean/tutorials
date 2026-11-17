#pragma once

#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPropPicker.h>
#include <vtkCellPicker.h>
#include <thread>
#include <future>
#include <atomic>

#ifdef __EMSCRIPTEN__
    #if VTK_MAJOR_VERSION <=9 && VTK_MINOR_VERSION < 3
    #include "vtkSDL2OpenGLRenderWindow.h"
    #include "vtkSDL2RenderWindowInteractor.h"
    #else
    #include "vtkWebAssemblyOpenGLRenderWindow.h"
    #include "vtkWebAssemblyRenderWindowInteractor.h"
    #endif
#endif // __EMSCRIPTEN__

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

class Worker
{
public:
    Worker();
    ~Worker();

    void Init();
    void Start();
    void SimuComplexTaskAsync();
protected:
#ifdef __EMSCRIPTEN__
    #if VTK_MAJOR_VERSION <=9 && VTK_MINOR_VERSION < 3
        vtkSPtr<vtkSDL2RenderWindowInteractor> m_RenderWindowInteractor;
        vtkSPtr<vtkSDL2OpenGLRenderWindow> m_RenderWindow;
    #else
        vtkSPtr<vtkWebAssemblyRenderWindowInteractor> m_RenderWindowInteractor;
        vtkSPtr<vtkWebAssemblyOpenGLRenderWindow> m_RenderWindow;
    #endif
#else
    vtkSPtr<vtkRenderWindowInteractor> m_RenderWindowInteractor;
    vtkSPtr<vtkRenderWindow> m_RenderWindow;
#endif
    vtkSPtr<vtkActor> m_Actor;
    vtkSPtr<vtkRenderer> m_Renderer;
    //std::thread *m_SubThread;
};