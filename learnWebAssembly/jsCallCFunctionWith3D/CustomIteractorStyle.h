#pragma once

#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTransform.h>

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
protected:
	  virtual void OnKeyPress() override;

    CustomIteractorStyle();
    ~CustomIteractorStyle() override;
};
