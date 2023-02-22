#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <wrl.h>

#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "SimpleMath.h"

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#include "SimpleMath.h"

#ifdef DIRECTX11GAMEFRAMEWORK_EXPORTS
#define GAMEFRAMEWORK_API __declspec(dllexport)
#else
#define GAMEFRAMEWORK_API __declspec(dllimport)
#endif
