#ifndef _ASNUKLEARIMPLIMENTATION_H_
#define _ASNUKLEARIMPLIMENTATION_H_

#include "../common/asCommon.h"
#include "../renderer/asShaderVariants.h"
#if ASTRENGINE_NUKLEAR
#ifdef __cplusplus
extern "C" {
#endif 

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT

/**
* @file
* @brief Implimentation of the Nuklear Immediate GUI library 
*/

/**
* @brief Return the internal main nuklear context
*/
ASEXPORT struct nk_context* asGetNuklearContextPtr();

/**
* @brief Initializes the nuklear backend
* @warning The engine ignite should handle this for you
*/
void asInitNk();
/**
* @brief Draw the nuklear user interface
* @warning The engine update should handle this for you
*/
void asNkDraw();

/**
* @brief Reset nuklear for next frame
* @warning The engine update should handle this for you
*/
void asNkReset();

/**
* @brief Push an event into nuklear
* @warning The engine update should handle this for you
* @warning this should be fed a pointer to an SDL_Event
*/
void asNkPushEvent(void *pEvent);

/**
* @brief begin nuklear input (events)
* @warning The engine update should handle this for you
*/
void asNkBeginInput();
/**
* @brief end nuklear input (events)
* @warning The engine update should handle this for you
*/
void asNkEndInput();

/**
* @brief Shutdown the nuklear backend
* @warning The engine shutdown should handle this for you
*/
void asShutdownNk();

/**
* @For internal use by shader system
*/
asGfxPipelineHandle _asGenerateGfxPipeline_Nuklear(asBinReader* pShaderAsBin,
	asShaderTypeCodePath* pCodePaths,
	size_t codePathCount,
	const char* pipelineName,
	void* pUserData);

#endif

#ifdef __cplusplus
}
#endif
#endif