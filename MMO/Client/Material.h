#pragma once

#include "ConstBuffer.h"

class MaterialBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Vector4 diffuse;
		Vector4 specular;
		Vector4 ambient;
		Vector4 emissive;

		float shininess;

		int hasDiffuseMap;
		int hasSpecularMap;
		int hasNormalMap;
	} data;

	MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.diffuse = { 1, 1, 1, 1 };
		data.specular = { 1, 1, 1, 1 };
		data.ambient = { 1, 1, 1, 1 };
		data.emissive = { 0.3f, 0.3f, 0.3f, 0.1f };
		data.shininess = 30;

		data.hasDiffuseMap = 0;
		data.hasSpecularMap = 0;
		data.hasNormalMap = 0;
	}
};

class VertexShader;
class PixelShader;
class Texture;

class Material
{
public:
	Material();
	Material(wstring file);
	Material(VertexShader* vertexShader, PixelShader* pixelShader);
	~Material();

	void Set();

	void SetShader(wstring file);

	void SetDiffuseMap(wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);

	MaterialBuffer* GetBuffer() { return buffer; }

public:
	string name = "";

	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

	MaterialBuffer* buffer = nullptr;
	
	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;
};

