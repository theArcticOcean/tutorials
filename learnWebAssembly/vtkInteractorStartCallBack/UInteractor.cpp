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
bool UInteractor::ProcessEvent(void *arg)
{
  SDL_Event *event = reinterpret_cast<SDL_Event *>(arg);
  std::cout << "event type: " << event->type << std::endl;

  // SDL_DISPLAYEVENT   = 0x150,  /**< Display state change */  336
  // SDL_WINDOWEVENT    = 0x200, /**< Window state change */  512
  // SDL_MOUSEMOTION    = 0x400, /**< Mouse moved */  1024
  // SDL_MOUSEWHEEL,             /**< Mouse wheel motion */ 1027
  if (SDL_WINDOWEVENT == event->type)
  {
    std::cout << "window.event: " << (int)(event->window.event) << std::endl; // SDL_WINDOWEVENT_SHOWN is 1
    // 6, 5, 10, 12, 1
    // SDL_WINDOWEVENT_SIZE_CHANGED, SDL_WINDOWEVENT_RESIZED, SDL_WINDOWEVENT_ENTER, SDL_WINDOWEVENT_FOCUS_GAINED, SDL_WINDOWEVENT_SHOWN
  }

  // SDL_WINDOWEVENT_NONE,           /**< Never used */
  // SDL_WINDOWEVENT_SHOWN,          /**< Window has been shown */
  // SDL_WINDOWEVENT_HIDDEN,         /**< Window has been hidden */
  // SDL_WINDOWEVENT_EXPOSED,        /**< Window has been exposed and should be
  //                                     redrawn */
  // SDL_WINDOWEVENT_MOVED,          /**< Window has been moved to data1, data2
  //                                 */
  // SDL_WINDOWEVENT_RESIZED,        /**< Window has been resized to data1xdata2 */  //5
  // SDL_WINDOWEVENT_SIZE_CHANGED,   /**< The window size has changed, either as
  //                                     a result of an API call or through the
  //                                     system or user changing the window size. */
  // SDL_WINDOWEVENT_MINIMIZED,      /**< Window has been minimized */
  // SDL_WINDOWEVENT_MAXIMIZED,      /**< Window has been maximized */
  // SDL_WINDOWEVENT_RESTORED,       /**< Window has been restored to normal size
  //                                      and position */
  // SDL_WINDOWEVENT_ENTER,          /**< Window has gained mouse focus */  //10
  // SDL_WINDOWEVENT_LEAVE,          /**< Window has lost mouse focus */
  // SDL_WINDOWEVENT_FOCUS_GAINED,   /**< Window has gained keyboard focus */ //12
  // SDL_WINDOWEVENT_FOCUS_LOST,     /**< Window has lost keyboard focus */


  return vtkSDL2RenderWindowInteractor::ProcessEvent(arg);
}