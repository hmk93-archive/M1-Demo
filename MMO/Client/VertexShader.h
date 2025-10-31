#pragma once

#include "Shader.h"

class VertexShader : public Shader
{
private:
	friend class Shader;

	VertexShader(wstring file, string entry);
	~VertexShader();

public:
	virtual void Set() override;

private:
	void CreateInputLayout();

private:
	ComPtr<ID3D11VertexShader> _shader;
	ComPtr<ID3D11InputLayout> _inputLayout;
};

