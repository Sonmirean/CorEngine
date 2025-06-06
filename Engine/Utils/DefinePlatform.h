#pragma once



#if defined(WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#define CORENGINE_PLATFORM_CODE 1;

#elif defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#define CORENGINE_PLATFORM_CODE 2;

#elif defined(macintosh)
#define VK_USE_PLATFORM_MACOS_MVK
#define CORENGINE_PLATFORM_CODE 3;

#endif


