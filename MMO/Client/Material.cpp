#include "pch.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "ModelReader.h"

Material::Material()
{
	buffer = new MaterialBuffer();
}

Material::Material(wstring file)
{
	vertexShader = Shader::AddVS(file);
	pixelShader = Shader::AddPS(file);

	buffer = new MaterialBuffer();
}

Material::Material(VertexShader* vertexShader, PixelShader* pixelShader)
	: vertexShader(vertexShader)
	, pixelShader(pixelShader)
{
	buffer = new MaterialBuffer();
}

Material::~Material()
{
	delete buffer;
}

void Material::Set()
{
	if (diffuseMap)
		diffuseMap->PSSet(0);

	if (specularMap)
		specularMap->PSSet(1);

	if (normalMap)
		normalMap->PSSet(2);

	buffer->SetPSBuffer(1);

	vertexShader->Set();
	pixelShader->Set();
}

void Material::SetShader(wstring file)
{
	vertexShader = Shader::AddVS(file);
	pixelShader = Shader::AddPS(file);
}

void Material::SetDiffuseMap(wstring file)
{
	diffuseMap = Texture::Add(file);
	buffer->data.hasDiffuseMap = 1;
}

void Material::SetSpecularMap(wstring file)
{
	specularMap = Texture::Add(file);
	buffer->data.hasSpecularMap = 1;
}

void Material::SetNormalMap(wstring file)
{
	normalMap = Texture::Add(file);
	buffer->data.hasNormalMap = 1;
}

