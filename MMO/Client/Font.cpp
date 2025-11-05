#include "pch.h"
#include "Font.h"
#include "Device.h"

Font::Font()
{
	if (FAILED((DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&writeFactory))))
	{
		__debugbreak();
	}

	if (FAILED((D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory))))
	{
		__debugbreak();
	}

	IDXGIDevice* dxgiDevice;
	if (FAILED(Device::Get().GetDevice()->QueryInterface(&dxgiDevice)))
	{
		__debugbreak();
	}

	if (FAILED(factory->CreateDevice(dxgiDevice, &device)))
	{
		__debugbreak();
	}

	dxgiDevice->Release();
	
	if (FAILED(device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &context)))
	{
		__debugbreak();
	}

	CreateTargetBitmap();
}

Font::~Font()
{
	brush->Release();
	format->Release();
	
	factory->Release();
	writeFactory->Release();

	ReleaseTargetBitmap();

	context->Release();
	device->Release();
}

void Font::Add(Vector4 color, float fontSize)
{
	D2D1::ColorF colorF = D2D1::ColorF(color.x, color.y, color.z, color.w);	// alpha Ãß°¡ (color.w)
	context->CreateSolidColorBrush(colorF, &brush);

	if (FAILED(writeFactory->CreateTextFormat(
		font.c_str(), nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"ko", &format
	)))
	{
		__debugbreak();
	}
}

void Font::RenderText(wstring text, Vector3 pos, float fontSize, DXGI_RGBA color, UINT start, UINT length, Vector2 rectSize)
{
	// Create layout
	writeFactory->CreateTextLayout(text.c_str(), (UINT)text.size(), format,
		rectSize.x, rectSize.y, &layout);
	// Swap color
	DXGI_RGBA oldColor = brush->GetColor();	// DXGI_RGBA == D2D1_COLOR_F
	brush->SetColor(color);
	// Set Font Size (start ~ start+length)
	DWRITE_TEXT_RANGE range{}; // UINT32 startPosition, UINT32 length;
	range.startPosition = start;
	if (length == 0)
		range.length = (UINT)text.size();
	else
		range.length = length;
	layout->SetFontSize(fontSize, range);
	// SetPos
	D2D1_POINT_2F origin = { pos.x, pos.y };
	D2D1_DRAW_TEXT_OPTIONS option = D2D1_DRAW_TEXT_OPTIONS_NONE;
	// Render Text 
	context->DrawTextLayout(origin, layout, brush, option);
	// Reset Color
	brush->SetColor(oldColor);
	// Relese Layout //
	layout->Release();
}

void Font::CreateTargetBitmap()
{
	IDXGISurface* dxgiSurface;
	if (FAILED((Device::Get().GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiSurface))))
	{
		__debugbreak();
	}

	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	if (FAILED(context->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap)))
	{
		__debugbreak();
	}

	dxgiSurface->Release();

	context->SetTarget(targetBitmap);
}

void Font::ReleaseTargetBitmap()
{
	context->SetTarget(nullptr);
	targetBitmap->Release();
}
