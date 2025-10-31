#pragma once

#include "Shader.h"

class HullShader : public Shader
{
private:
	friend class Shader;

	HullShader(wstring file, string entry);
	~HullShader();

public:
	virtual void Set() override;

private:
	ComPtr<ID3D11HullShader> _shader;
};