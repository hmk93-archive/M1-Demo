#pragma once

struct Vertex
{
	Vector3 position;

	Vertex() : position(0, 0, 0)
	{
	}

	Vertex(float x, float y, float z) : position(x, y, z)
	{
	}
};

struct VertexSize
{
	Vector3 position;
	Vector2 size;

	VertexSize() : position(0, 0, 0), size(0, 0)
	{
	}
};

struct VertexColor
{
	Vector3 position;
	Vector4 color;

	VertexColor() : position(0, 0, 0), color(1, 1, 1, 1)
	{
	}

	VertexColor(Vector3 position, Vector4 color) : position(position), color(color)
	{
	}
};

struct VertexUVNormal
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;

	VertexUVNormal() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0)
	{
	}
};

struct VertexUVNormalTangent
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;

	VertexUVNormalTangent() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0)
	{
	}
};

struct VertexUVNormalTangentAlpha
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
	float alpha[4];

	VertexUVNormalTangentAlpha()
		: position(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0), alpha{}
	{
	}
};

struct VertexUVNormalTangentBlend
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
	Vector4 indices;
	Vector4 weights;

	VertexUVNormalTangentBlend() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0), indices(0, 0, 0, 0), weights(0, 0, 0, 0)
	{
	}
};
