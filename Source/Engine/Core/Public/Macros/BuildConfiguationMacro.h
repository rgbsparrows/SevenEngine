#pragma once

#if defined(SE_BUILD_DEBUG)
#define WITH_DEBUG_CODE 1
#define WITH_DEVELOPMENT_CODE 1
#elif defined(SE_BUILD_DEVELOPMENT)
#define WITH_DEBUG_CODE 1
#define WITH_DEVELOPMENT_CODE 1
#elif defined(SE_BUILD_SHIPPING)
#define WITH_DEBUG_CODE 0
#define WITH_DEVELOPMENT_CODE 0
#elif defined(SE_BUILD_TEST)
#define WITH_DEBUG_CODE 0
#define WITH_DEVELOPMENT_CODE 1
#else
#error error build configuation
#endif
