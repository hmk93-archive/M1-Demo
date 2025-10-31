#pragma once

#include "Shader.h"

class DomainShader : public Shader
{
private:
	friend class Shader;

	DomainShader(wstring file, string entry);
	~DomainShader();

public:
	virtual void Set() override;

private:
	ComPtr<ID3D11DomainShader> _shader;
};