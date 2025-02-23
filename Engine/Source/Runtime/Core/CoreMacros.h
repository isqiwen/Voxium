#pragma once

#if defined(_WIN32) || defined(_WIN64)
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#else
#ifdef CORE_EXPORTS
#define CORE_API __attribute__((visibility("default")))
#else
#define CORE_API
#endif
#endif

#ifdef DEBUG
#define DEBUG_CALL(fn, ...) fn(__VA_ARGS__)
#else
#define DEBUG_CALL(fn, ...) ((void)0)
#endif
