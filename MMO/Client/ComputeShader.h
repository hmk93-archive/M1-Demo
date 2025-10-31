#pragma once

#include "Shader.h"

class ComputeShader : public Shader
{
private:
	friend class Shader;

	ComputeShader(wstring file, string entry);
	~ComputeShader();

public:
	virtual void Set() override;

private:
	ComPtr<ID3D11ComputeShader> _shader;
};