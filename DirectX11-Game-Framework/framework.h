#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#ifdef DIRECTX11GAMEFRAMEWORK_EXPORTS
#define GAMEFRAMEWORK_API __declspec(dllexport)
#else
#define GAMEFRAMEWORK_API __declspec(dllimport)
#endif
