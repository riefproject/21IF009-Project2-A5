#ifndef REESTRUCT_EXPORT_H
#define REESTRUCT_EXPORT_H

// Windows build
#if defined(_WIN32)
#if defined(__TINYC__)
#define __declspec(x) __attribute__((x))
#endif

#if defined(BUILD_LIBTYPE_SHARED)
#define RSTAPI __declspec(dllexport)
#elif defined(USE_LIBTYPE_SHARED)
#define RSTAPI __declspec(dllimport)
#else
#define RSTAPI                  
#endif

// Unix-like (Linux, macOS)
#else
#if defined(BUILD_LIBTYPE_SHARED)
#define RSTAPI __attribute__((visibility("default")))
#else
#define RSTAPI
#endif
#endif

#endif // REESTRUCT_EXPORT_H
