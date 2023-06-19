#pragma once

#ifdef __EMSCRIPTEN__
#define vtkBaseWindowInteractor vtkSDL2RenderWindowInteractor
#else
#define vtkBaseWindowInteractor vtkRenderWindowInteractor
#endif

#ifdef __EMSCRIPTEN__
#include <vtkSDL2RenderWindowInteractor.h>
#else
#include <vtkRenderWindowInteractor.h>
#endif

#include <vtkSetGet.h>


class UInteractor: public vtkBaseWindowInteractor
{
public:
  static UInteractor* New();

  vtkTypeMacro(UInteractor, vtkBaseWindowInteractor);

  void ProcessEvents() override;
protected:
  UInteractor();
  ~UInteractor() override;

  bool ProcessEvent(void* event);  
};
