#pragma once

#include "Shader.h"

class PixelShader : public Shader
{
private:
	friend class Shader;

	PixelShader(wstring file, string entry);
	~PixelShader();

public:
	virtual void Set() override;

private:
	ComPtr<ID3D11PixelShader> _shader;
};

