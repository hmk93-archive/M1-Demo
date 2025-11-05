#pragma once

class Font
{
	DECLARE_SINGLETON(Font);
public:
	void Add(Vector4 color = Vector4(1.0f), float fontSize = 20.0f);
	void RenderText(wstring text, Vector3 pos, float fontSize = 20.0f, DXGI_RGBA color = DXGI_RGBA{ 1.0f, 1.0f, 1.0f, 1.0f }, 
		UINT start = 0, UINT length = 0, Vector2 rectSize = { 100, 50 });

	ID2D1DeviceContext* GetDC() { return _context; }

	void CreateTargetBitmap();
	void ReleaseTargetBitmap();

private:
	ID2D1Factory1* _factory = nullptr;
	IDWriteFactory* _writeFactory = nullptr;

	ID2D1Device* _device = nullptr;
	ID2D1DeviceContext* _context = nullptr;

	ID2D1Bitmap1* _targetBitmap = nullptr;

	ID2D1SolidColorBrush* _brush = nullptr;
	IDWriteTextFormat* _format = nullptr;
	IDWriteTextLayout* _layout = nullptr;

	wstring font = L"Font Test";
};

