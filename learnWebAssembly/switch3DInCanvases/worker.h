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

#ifdef __EMSCRIPTEN__
#include "vtkSDL2OpenGLRenderWindow.h"
#include "vtkSDL2RenderWindowInteractor.h"
#endif // __EMSCRIPTEN__

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

class Worker
{
public:
    Worker();

    void Init();
    void Start();

    void Terminate();
    void ReStart();
protected:

#ifdef __EMSCRIPTEN__
    vtkSPtr<vtkSDL2RenderWindowInteractor> m_RenderWindowInteractor;
    vtkSPtr<vtkSDL2OpenGLRenderWindow> m_RenderWindow;
#else
    vtkSPtr<vtkRenderWindowInteractor> m_RenderWindowInteractor;
    vtkSPtr<vtkRenderWindow> m_RenderWindow;
#endif
};