#pragma once

#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTransform.h>

#include "point.hpp"

#define CPP_SET_MACRO(name,type) \
  void Set##name(type _arg) \
  { \
    if (this->name != _arg) \
    { \
    this->name = _arg; \
    } \
  }

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

class CustomIteractorStyle: public vtkInteractorStyleTrackballCamera
{
public:
    static CustomIteractorStyle *New(){ return new CustomIteractorStyle(); }
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;
    void Rotate() override;
    void Pan() override;

    void Init();

    CPP_SET_MACRO( m_LeftRender, vtkSmartPointer<vtkRenderer> );
    CPP_SET_MACRO( m_RightRender, vtkSmartPointer<vtkRenderer> );
protected:
    CustomIteractorStyle();
    ~CustomIteractorStyle() override;
    void KeepSameCameraStatus();

    vtkSmartPointer<vtkRenderer> m_LeftRender;
    vtkSmartPointer<vtkRenderer> m_RightRender;
    vtkSmartPointer<vtkTransform> m_LastTrans;
};
