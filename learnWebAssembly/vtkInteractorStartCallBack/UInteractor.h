#pragma once

#include <vtkSDL2RenderWindowInteractor.h>
#include <vtkSetGet.h>

class UInteractor: public vtkSDL2RenderWindowInteractor
{
public:
  static UInteractor* New();

  vtkTypeMacro(UInteractor, vtkSDL2RenderWindowInteractor);

  void ProcessEvents() override;
protected:
  UInteractor();
  ~UInteractor() override;

  bool ProcessEvent(void* event);  
};