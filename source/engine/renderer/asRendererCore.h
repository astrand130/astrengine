#ifndef _ASRENDERERCORE_H_
#define _ASRENDERERCORE_H_

#include "../common/asCommon.h"
#include "../resource/asResource.h"
#ifdef __cplusplus
extern "C" {
#endif 

#define AS_MAX_INFLIGHT 2

/**
* @file
* @brief The mid level (above the API but below the ECS/postfx/etc) core of the engine's renderer
* Its job is not to abstract/encapsulate every aspect of rendering APIs but rather provide only meaninful and useful utilities
* to tasks such as texture/buffer resource management/interfacing with programmable shaders and materials
* each subsystem of the renderer is responsible for implimenting rendering algorithms (often using native calls) for each backend/API
*/
typedef enum {
	AS_GFXAPI_NULL,
	AS_GFXAPI_VULKAN,
	AS_GFXAPI_OPENGL,
	AS_GFXAPI_DIRECTX,
	AS_GFXAPI_METAL,
	AS_GFXAPI_MAX = UINT32_MAX
} asGfxAPIs;

#if ASTRENGINE_VK
/*Current Graphics API*/
#define AS_GFXAPI AS_GFXAPI_VULKAN
/*Major API Version (example: (dx)12, (vk)1, (gl)45, etc)*/
#define AS_GFXAPI_VERSION 1
#endif

/**
* @brief Get the Main window as a SDL Window
*/
ASEXPORT struct SDL_Window* asGetMainWindowPtr();

/**
* @brief Initializes the window and renderer
* @warning The engine ignite should handle this for you
*/
ASEXPORT void asInitGfx(asAppInfo_t *pAppInfo, void* pCustomWindow);
/**
* @brief Render a single frame
* @warning The engine update should handle this for you
*/
ASEXPORT void asGfxRenderFrame();

ASEXPORT void asGfxInternalDebugDraws();

/**
* @brief Trigger a window resize event, the renderer will attempt to adjust to it
*/
ASEXPORT void asGfxTriggerResizeEvent();

/**
* @brief Tells the program to skip frame drawing
*/
ASEXPORT void asGfxSetDrawSkip(bool skip);
/**
* @brief Shutdown the window and renderer
* @warning The engine shutdown should handle this for you
*/
ASEXPORT void asShutdownGfx();

/*Textures*/

/**
* @brief Types of textures
*/
typedef enum {
	AS_TEXTURETYPE_2D,
	AS_TEXTURETYPE_2DARRAY,
	AS_TEXTURETYPE_CUBE,
	AS_TEXTURETYPE_CUBEARRAY,
	AS_TEXTURETYPE_3D,
	AS_TEXTURETYPE_COUNT,
	AS_TEXTURETYPE_MAX = UINT32_MAX
} asTextureType;

/**
* @brief Common image color formats for rendering
*/
typedef enum {
	AS_COLORFORMAT_DEPTH,
	AS_COLORFORMAT_DEPTH_LP,
	AS_COLORFORMAT_DEPTH_STENCIL,
	AS_COLORFORMAT_RGBA8_UNORM,
	AS_COLORFORMAT_RGBA16_UNORM,
	AS_COLORFORMAT_RGBA16_UINT,
	AS_COLORFORMAT_RGBA16_SFLOAT,
	AS_COLORFORMAT_RGBA32_SFLOAT,
	AS_COLORFORMAT_A2R10G10B10_UNORM,
	AS_COLORFORMAT_A2R10G10B10_SNORM,
	AS_COLORFORMAT_B10G11R11_UFLOAT,
	AS_COLORFORMAT_R8_UNORM,
	AS_COLORFORMAT_R16_SFLOAT,
	AS_COLORFORMAT_R16_UNORM,
	AS_COLORFORMAT_R32_SFLOAT,
	AS_COLORFORMAT_RG16_SFLOAT,
	AS_COLORFORMAT_RG32_SFLOAT,
	AS_COLORFORMAT_RGB16_SFLOAT,
	AS_COLORFORMAT_RGB32_SFLOAT,
	AS_COLORFORMAT_RGBA32_UINT,
	AS_COLORFORMAT_BC1_RGBA_UNORM_BLOCK,
	AS_COLORFORMAT_BC3_UNORM_BLOCK,
	AS_COLORFORMAT_BC5_UNORM_BLOCK,
	AS_COLORFORMAT_BC6H_UFLOAT_BLOCK,
	AS_COLORFORMAT_BC7_UNORM_BLOCK,
	AS_COLORFORMAT_COUNT,
	AS_COLORFORMAT_MAX = UINT32_MAX
} asColorFormat;

typedef enum {
	AS_COLORCHANNEL_R = 1 << 0,
	AS_COLORCHANNEL_G = 1 << 1,
	AS_COLORCHANNEL_B = 1 << 2,
	AS_COLORCHANNEL_A = 1 << 3,
	AS_COLORCHANNEL_COLOR =
	AS_COLORCHANNEL_R |
	AS_COLORCHANNEL_G |
	AS_COLORCHANNEL_B,
	AS_COLORCHANNEL_ALL =
	AS_COLORCHANNEL_COLOR |
	AS_COLORCHANNEL_A,
	AS_COLORCHANNEL_MAX = UINT32_MAX
} asColorChannelsFlags;

/**
* @brief Texture usage flags
*/
typedef enum {
	AS_TEXTUREUSAGE_TRANSFER_DST = 1 << 0,
	AS_TEXTUREUSAGE_TRANSFER_SRC = 1 << 1,
	AS_TEXTUREUSAGE_SAMPLED = 1 << 2,
	AS_TEXTUREUSAGE_RENDERTARGET = 1 << 3,
	AS_TEXTUREUSAGE_DEPTHBUFFER = 1 << 4,
	AS_TEXTUREUSAGE_MAX = UINT32_MAX
} asTextureUsageFlags;

/**
* @brief The way in which the cpu will access the resource and its allocation behavior
*/
typedef enum {
	AS_GPURESOURCEACCESS_DEVICE, /**< Staging uploads are required so the resource will be updated infrequently but faster for GPU*/
	AS_GPURESOURCEACCESS_STAGING, /**< Use this for staging uploads, not buffered for rendering without artifacts*/
	AS_GPURESOURCEACCESS_STREAM, /**< Every frame the data might be different and the host can access this directly*/
	AS_GPURESOURCEACCESS_COUNT,
	AS_GPURESOURCEACCESS_MAX = UINT32_MAX
} asGpuResourceUploadType;

/**
* @brief Specifies a region of content in an image
*/
typedef struct {
	uint32_t bufferStart; /**< Starting position of this region in the buffer*/
	uint32_t offset[3]; /**< UVW Offset of the region in pixels*/
	uint32_t extent[3]; /**< UVW Extent of the region in pixels*/
	uint32_t mipLevel; /**< Which mip level this region belongs to*/
	uint32_t layer; /**< Which layer this region belongs to*/
	uint32_t layerCount; /**< How many layers this region has in it*/
} asTextureContentRegion_t;

#define AS_TEXTURE_MAX_REGIONS 16

/**
* @brief Description for a texture resource
*/
typedef struct {
	asTextureType type; /**< What type of texture is this*/
	asGpuResourceUploadType cpuAccess; /**< CPU access info*/
	asColorFormat format; /**< Color format of the texture*/
	int32_t usageFlags; /**< Usage of the texture (default assumes its sampled in a shader)*/
	uint32_t width; /**< Width in pixels*/
	uint32_t height; /**< Height in pixels*/
	uint32_t depth; /**< Depth in pixels (or amount of layers in an array)*/
	uint32_t mips; /**< Number of mip levels*/
	size_t initialContentsBufferSize; /**< The size of the data in the buffer to be uploaded*/
	const void* pInitialContentsBuffer; /**< Pointer to the initial contents of the texture (if NULL nothing will be uploaded)*/
	size_t initialContentsRegionCount; /**< The amount of regions to copy into*/
	asTextureContentRegion_t* pInitialContentsRegions; /**< Used to map buffer data to parts of the texture image (if NULL look into asTextureDesc_t::arrInitialContentsRegions)*/
	asTextureContentRegion_t arrInitialContentsRegions[AS_TEXTURE_MAX_REGIONS]; /**< Used to map buffer data to parts of the texture image (if NULL same above)*/
	const char* pDebugLabel; /**< Debug name of the texture that should appear in debuggers*/
} asTextureDesc_t;

/**
* @brief Set the defaults for a texture
*/
ASEXPORT asTextureDesc_t asTextureDesc_Init();

/**
* @brief Calculate the pitch of a texture
*/
ASEXPORT uint32_t asTextureCalcPitch(asColorFormat format, uint32_t width);

/**
* @brief Native size of a depth buffer
*/
ASEXPORT uint32_t asGetDepthFormatSize(asColorFormat preference);

/**
* @brief Hanlde to a texture resource
*/
typedef asHandle_t asTextureHandle_t;

/**
* @brief API independent mechanism for creating texture resources
* @warning not guaranteed to be threadsafe
*/
ASEXPORT asTextureHandle_t asCreateTexture(asTextureDesc_t *pDesc);

/**
* @brief API independent mechanism for releasing texture resources
* @warning not guaranteed to be immideate/threadsafe
*/
ASEXPORT void asReleaseTexture(asTextureHandle_t hndl);

/**
* @brief Maximum number of textures
*/
#define AS_MAX_TEXTURES 1024

/*Buffers*/

/**
* @brief Texture usage flags
*/
typedef enum {
	AS_BUFFERUSAGE_TRANSFER_DST = 1 << 0,
	AS_BUFFERUSAGE_TRANSFER_SRC = 1 << 1,
	AS_BUFFERUSAGE_INDEX = 1 << 2,
	AS_BUFFERUSAGE_VERTEX = 1 << 3,
	AS_BUFFERUSAGE_INDIRECT = 1 << 4,
	AS_BUFFERUSAGE_UNIFORM = 1 << 5,
	AS_BUFFERUSAGE_STORAGE = 1 << 6,
	AS_BUFFERUSAGE_MAX = UINT32_MAX
} asBufferUsageFlags;

/**
* @brief Description for a buffer resource
*/
typedef struct {
	asGpuResourceUploadType cpuAccess; /**< CPU access info*/
	int32_t usageFlags; /**< Usage of the buffer*/
	uint32_t bufferSize; /**< Size of the buffer in bytes*/
	size_t initialContentsBufferSize; /**< The size of the data in the buffer to be uploaded*/
	void* pInitialContentsBuffer; /**< Pointer to the initial contents of the buffer (if NULL nothing will be uploaded)*/
	const char* pDebugLabel; /**< Debug name of the texture that should appear in debuggers*/
} asBufferDesc_t;

/**
* @brief Set the defaults for a buffer
*/
ASEXPORT asBufferDesc_t asBufferDesc_Init();

/**
* @brief Hanlde to a buffer resource
*/
typedef asHandle_t asBufferHandle_t;

/**
* @brief API independent mechanism for creating buffer resources
* @warning not guaranteed to be threadsafe
*/
ASEXPORT asBufferHandle_t asCreateBuffer(asBufferDesc_t *pDesc);

/**
* @brief API independent mechanism for releasing buffer resources
* @warning not guaranteed to be immideate/threadsafe
*/
ASEXPORT void asReleaseBuffer(asBufferHandle_t hndl);

/**
* @brief Maximum number of buffers
*/
#define AS_MAX_BUFFERS 2048

/*Screen Management*/

/**
* @brief Get render dimensions for a viewport 
*/
ASEXPORT asResults asGetRenderDimensions(int screenId, bool dynamicRes, int32_t* pWidth, int32_t* pHeight);

/*Global Shader Properties*/
#define AS_MAX_GLOBAL_CUSTOM_PARAMS 8
ASEXPORT asResults asSetGlobalCustomShaderParam(int slot, float values[4]);
ASEXPORT asResults asSetGlobalShaderDebugMode(int mode);
ASEXPORT asResults asSetGlobalShaderTime(double time);

/*Misc*/
/**
* @brief Types of queues
*/
typedef enum {
	AS_GFXSTAGE_ALL_GRAPHICS,
	AS_GFXSTAGE_COMPUTE,
	AS_GFXSTAGE_TRANSFER,
	AS_GFXSTAGE_RAYTRACE,
	AS_GFXSTAGE_MAX = UINT32_MAX
} asGfxStage;

#ifdef __cplusplus
}
#endif
#include "asRenderFx.h"

#endif