#pragma once

#include "asCommon.h"

/**
* @file
* @brief Handles operating system events
*/

/**
* @brief Polls the operating system for events and populates subsystem command buffers
* @warning Don't touch this the engine loop should handle this for you
*/
ASEXPORT void asPollOSEvents();