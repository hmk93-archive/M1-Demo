#pragma once

class MatrixBuffer;
class Material;
class Mesh;

class Transform
{
public:
	Transform(string name = "Untagged");
	virtual ~Transform();

	Vector3 Forward() { return _world.Backward(); }
	Vector3 Up() { return _world.Up(); }
	Vector3 Right() { return _world.Right(); }

	Vector3 GlobalPos() { return _globalPosition; }
	Vector3 GlobalRot() { return _globalRotation; }
	Vector3 GlobalScale() { return _globalScale; }

	void UpdateWorld();

	void SetParent(Matrix* parent) { _parent = parent; }
	void SetWorldBuffer(UINT slot = 0);
	void SetTarget(Transform* target, Vector3 offset = Vector3(0.0f));

	void RenderAxis();

	Matrix* GetWorld() { return &_world; }

private:
	void CreateAxis();

public:
	string tag = "";
	Vector3 position = {};
	Vector3 rotation = {};
	Vector3 scale = Vector3(1.0f);
	bool isActive = true;

protected:
	Vector3 _globalPosition = {};
	Vector3 _globalRotation = {};
	Vector3 _globalScale = Vector3(1.0f);

	Matrix _world = Matrix::Identity;

	Matrix* _parent = nullptr;

	MatrixBuffer* _worldBuffer = nullptr;

	Transform* _target = nullptr;

private:
	Material* _material = nullptr;
	Mesh* _mesh = nullptr;

	MatrixBuffer* _transformBuffer = nullptr;

	vector<VertexColor> _vertices = {};
	vector<UINT> _indices = {};

	Vector3 _targetOffset = Vector3(0.0f);
};

