// visibility.h
#pragma once

#if defined(_MSC_VER)
// Visual Studio
#ifdef BUILDING_MYLIB
#define API_PUBLIC __declspec(dllexport)
#define API_PRIVATE
#else
#define API_PUBLIC __declspec(dllimport)
#define API_PRIVATE
#endif
#elif defined(__GNUC__)
// GCC/Clang
#ifdef BUILDING_MYLIB
#define API_PUBLIC __attribute__((visibility("default")))
#define API_PRIVATE __attribute__((visibility("hidden")))
#else
#define API_PUBLIC
#define API_PRIVATE
#endif
#else
#define API_PUBLIC
#define API_PRIVATE
#endif