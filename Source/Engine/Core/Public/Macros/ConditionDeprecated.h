#pragma once

#include "UtilMacros.h"

#include <version>

/*
 * 有一些功能会在未来，在预期中被添加，但暂时由于时间原因，优先级在其他任务之后，所以增加条件废弃功能，以便于后期完成相应功能时对其进行重写
 * 同时这里也包含一些还没有正式实现的cpp新特性，以便于未来用新的cpp特性生成更优质的代码
 */

//vs IntelliSense 对concept的支持还有一些问题，所以暂时先关闭concept的使用，虽然现在concept在vs中的确可用
#if 0 && defined(__cpp_concepts) && defined(__cpp_lib_concepts)
#define DEPRECATED_WHEN_CONCEPT_AVAILABLE(message) DEPRECATED("This should be rewritten when concept is available" message)
#define REWRITE_WHEN_CONCEPT_AVAILABLE(message) COMPILE_MESSAGE("This should be rewritten when concept is available" message)
#pragma message(FILE_LOC "This should be removed after the rewrite is finished")
#else
#define DEPRECATED_WHEN_CONCEPT_AVAILABLE(...)
#define REWRITE_WHEN_CONCEPT_AVAILABLE(...)
#endif

#ifdef SE_LOG_AVILABLE
#define DEPRECATED_WHEN_SE_LOG_AVAILABLE(message) DEPRECATED("This should be rewritten when SE_LOG is available" message)
#define REWRITE_WHEN_SE_LOG_AVAILABLE(message) COMPILE_MESSAGE("This should be rewritten when SE_LOG is available" message)
#pragma message(FILE_LOC "This should be removed after the rewrite is finished")
#else
#define DEPRECATED_WHEN_SE_LOG_AVAILABLE(...)
#define REWRITE_WHEN_SE_LOG_AVAILABLE(...)
#endif

#ifdef SE_STREAM_AVILABLE
#define DEPRECATED_WHEN_SE_STREAM_AVAILABLE(message) DEPRECATED("This should be rewritten when SE_STREAM is available" message)
#define REWRITE_WHEN_SE_STREAM_AVAILABLE(message) COMPILE_MESSAGE("This should be rewritten when SE_STREAM is available" message)
#pragma message(FILE_LOC "This should be removed after the rewrite is finished")
#else
#define DEPRECATED_WHEN_SE_STREAM_AVAILABLE(...)
#define REWRITE_WHEN_SE_STREAM_AVAILABLE(...)
#endif

#ifdef FMT_LIBRARY_AVILABLE
#define DEPRECATED_WHEN_FMT_LIBRARY_AVAILABLE(message) DEPRECATED("This should be rewritten when FMT LIBRARY is available" message)
#define REWRITE_WHEN_FMT_LIBRARY_AVAILABLE(message) COMPILE_MESSAGE("This should be rewritten when FMT LIBRARY is available" message)
#pragma message(FILE_LOC "This should be removed after the rewrite is finished")
#else
#define DEPRECATED_WHEN_FMT_LIBRARY_AVAILABLE(...)
#define REWRITE_WHEN_FMT_LIBRARY_AVAILABLE(...)
#endif

#ifdef SE_MONOLITHIC_MODULE_AVILABLE
#define DEPRECATED_WHEN_SE_MONOLITHIC_MODULE_AVAILABLE(message) DEPRECATED("This should be rewritten when SE MONOLITHIC MODULE is available" message)
#define REWRITE_WHEN_SE_MONOLITHIC_MODULE_AVAILABLE(message) COMPILE_MESSAGE("This should be rewritten when SE MONOLITHIC MODULE is available" message)
#pragma message(FILE_LOC "This should be removed after the rewrite is finished")
#else
#define DEPRECATED_WHEN_SE_MONOLITHIC_MODULE_AVAILABLE(...)
#define REWRITE_WHEN_SE_MONOLITHIC_MODULE_AVAILABLE(...)
#endif

#ifdef SE_GAME_PROJECT_RELATED_AVILABLE
#define DEPRECATED_WHEN_GAME_PROJECT_RELATED_AVAILABLE(message) DEPRECATED("This should be rewritten when GAME_PROJECT_RELATED is available" message)
#define REWRITE_WHEN_GAME_PROJECT_RELATED_AVAILABLE(message) COMPILE_MESSAGE("This should be rewritten when GAME_PROJECT_RELATED is available" message)
#pragma message(FILE_LOC "This should be removed after the rewrite is finished")
#else
#define DEPRECATED_WHEN_GAME_PROJECT_RELATED_AVAILABLE(...)
#define REWRITE_WHEN_GAME_PROJECT_RELATED_AVAILABLE(...)
#endif
