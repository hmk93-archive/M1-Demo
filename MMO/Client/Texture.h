#pragma once

class Texture
{
private:
	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image);
	~Texture();

public:
	static Texture* Add(wstring file);
	static Texture* Load(wstring file);

	static void Delete();

	void PSSet(UINT slot);
	void DSSet(UINT slot);

	vector<Vector4> ReadPixels();

	UINT Width() { return (UINT)_image.GetMetadata().width; }
	UINT Height() { return (UINT)_image.GetMetadata().height; }

	ID3D11ShaderResourceView* GetSRV() { return _srv.Get(); }

private:
	ScratchImage _image = {};

	ComPtr<ID3D11ShaderResourceView> _srv;

	static map<wstring, Texture*> s_textureCache;
};

