#pragma once

#if defined(SE_BUILD_DEBUG)
#define WITH_DEBUG_CODE 1
#define WITH_TEST_CODE 1
#define WITH_PROFILE_CODE 1
#elif defined(SE_BUILD_SHIPPING)
#define WITH_DEBUG_CODE 0
#define WITH_TEST_CODE 0
#define WITH_PROFILE_CODE 0
#elif defined(SE_BUILD_PROFILE)
#define WITH_DEBUG_CODE 0
#define WITH_TEST_CODE 1
#define WITH_PROFILE_CODE 1
#elif defined(SE_BUILD_TEST)
#define WITH_DEBUG_CODE 0
#define WITH_TEST_CODE 1
#define WITH_PROFILE_CODE 0
#else
#error error build configuation
#endif
