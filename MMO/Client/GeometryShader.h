#pragma once

#include "Shader.h"

class GeometryShader : public Shader
{
private:
	friend class Shader;

	GeometryShader(wstring file, string entry);
	~GeometryShader();

public:
	virtual void Set() override;

private:
	ComPtr<ID3D11GeometryShader> _shader;
};