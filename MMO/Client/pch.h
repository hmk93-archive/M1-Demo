#pragma once

#include "targetver.h"

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN            

// Windows Header Files
#include <windows.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// C++
#include <memory>
#include <algorithm>
#include <functional>

// D3D
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#include <d2d1_2.h>
#include <dwrite.h>
#include <DirectXTex/DirectXTex.h>
using namespace DirectX;
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

// Lib
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#pragma comment(lib, "Assimp/assimp-vc143-mt.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif

// Math
#include "SimpleMath.h"
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Ray;

// STL
#include <vector>
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>
using namespace std;

// Inner
#include "VertexLayouts.h"

// Macro
#define DECLARE_SINGLETON(ClassName)           \
public:                                        \
    static ClassName& Get() {                  \
        static ClassName instance;             \
        return instance;                       \
    }                                          \
private:                                       \
    ClassName();                               \
    ~ClassName();                              \
    ClassName(const ClassName&) = delete;      \
    ClassName& operator=(const ClassName&) = delete;

#define MAX_BONE 500
#define MAX_FRAME_KEY 600
#define MAX_INSTANCE 400

// Global


extern HWND g_hWnd;
extern UINT g_screenWidth;
extern UINT g_screenHeight;
