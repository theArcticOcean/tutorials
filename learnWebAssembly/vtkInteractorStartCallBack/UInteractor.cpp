#include "UInteractor.h"
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
#include <SDL.h>
#include <vector>

vtkStandardNewMacro(UInteractor);

UInteractor::UInteractor()
{

}

UInteractor::~UInteractor()
{

}

void UInteractor::ProcessEvents()
{
  // No need to do anything if this is a 'mapped' interactor
  if (!this->Enabled)
  {
    return;
  }

  SDL_Event event;
  std::vector<SDL_Event> events;

  // SDL generates continuous sequences of mouse motion events per frame,
  // let use only last event of each sequence

  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_MOUSEMOTION && !events.empty() && events.back().type == event.type)
    {
      events.back() = event;
    }
    else
    {
      events.push_back(event);
    }
  }

  for (SDL_Event ev : events)
  {
    if (this->ProcessEvent(&ev))
    {
      break;
    }
  }
}

/*
we can read logs on browser debug window.

Init!
StartWork!
event type: 512
event type: 1024
Uncaught (in promise) unwind
event type: 1024
event type: 1027

So we can notify the viewer once we receive event signal: SDL_WINDOWEVENT = 0x200 = 512
*/
bool UInteractor::ProcessEvent(void* arg)
{
  SDL_Event* event = reinterpret_cast<SDL_Event*>(arg);
  std::cout << "event type: " << event->type << std::endl;

  // SDL_DISPLAYEVENT   = 0x150,  /**< Display state change */  336
  // SDL_WINDOWEVENT    = 0x200, /**< Window state change */  512
  // SDL_MOUSEMOTION    = 0x400, /**< Mouse moved */  1024
  // SDL_MOUSEWHEEL,             /**< Mouse wheel motion */ 1027
  return vtkSDL2RenderWindowInteractor::ProcessEvent(arg);
}