#include "include/asEntry.h"
#include "include/asCommon.h"
#include "include/asOsEvents.h"
#include "include/asRendererCore.h"
#include <SDL.h>

bool gContinueLoop;

ASEXPORT void asShutdown(void)
{
	asShutdownGfx();
	SDL_Quit();
	asAllocShutdown_Linear();
	asDebugLog("astrengine Quit...");
}

ASEXPORT int asIgnite(int argc, char *argv[], asAppInfo_t *pAppInfo, void *pCustomWindow)
{
	/*Defaults*/
	if (!pAppInfo->pAppName)
		pAppInfo->pAppName = "UNTITLED";
	if(!pAppInfo->pGfxIniName)
		pAppInfo->pGfxIniName = "graphics.ini";

	/*Info*/
	asDebugLog("%s %d.%d.%d\n", pAppInfo->pAppName, pAppInfo->appVersion.major, pAppInfo->appVersion.minor, pAppInfo->appVersion.patch);
	asDebugLog("astrengine %d.%d.%d\n", ASTRENGINE_VERSION_MAJOR, ASTRENGINE_VERSION_MINOR, ASTRENGINE_VERSION_PATCH);

	/*Memory*/
	asAllocInit_Linear(100000);

	SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER);
	asInitGfx(pAppInfo, pCustomWindow);
	return 0; 
}

ASEXPORT int asLoopSingleShot()
{
	asPollOSEvents();
	return 0;
}

ASEXPORT int asEnterLoop()
{
	gContinueLoop = true;
	while (gContinueLoop)
	{
		asLoopSingleShot();
	}
	return 0;
}

ASEXPORT void asExitLoop()
{
	gContinueLoop = false;
}