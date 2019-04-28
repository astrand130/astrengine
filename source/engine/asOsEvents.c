#include "include/asOsEvents.h"
#include <SDL.h>

#include "include/asEntry.h"
#include "include/asRendererCore.h"
#if ASTRENGINE_NUKLEAR
#include "include/asNuklearImplimentation.h"
#endif

ASEXPORT void asPollOSEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			asExitLoop();
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_MINIMIZED:
				asGfxSetDrawSkip(true);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				asGfxSetDrawSkip(false);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				asGfxTriggerResizeEvent();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
#if ASTRENGINE_NUKLEAR
		asNkPushEvent(&event);
#endif
	}
}