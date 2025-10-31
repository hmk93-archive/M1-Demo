#pragma once

class Font
{
	DECLARE_SINGLETON(Font);
public:
	void Add(Vector4 color = Vector4(1.0f), float fontSize = 20.0f);
	void RenderText(wstring text, Vector3 pos, float fontSize = 20.0f, DXGI_RGBA color = DXGI_RGBA{ 1.0f, 1.0f, 1.0f, 1.0f },
		UINT start = 0, UINT length = 0, Vector2 rectSize = { 100, 50 });

	ID2D1DeviceContext* GetDC() { return context; }

private:
	ID2D1Factory1* factory = nullptr;
	IDWriteFactory* writeFactory = nullptr;

	ID2D1Device* device = nullptr;
	ID2D1DeviceContext* context = nullptr;

	ID2D1Bitmap1* targetBitmap = nullptr;

	ID2D1SolidColorBrush* brush = nullptr;
	IDWriteTextFormat* format = nullptr;
	IDWriteTextLayout* layout = nullptr;

	wstring font = L"Font Test";
};

