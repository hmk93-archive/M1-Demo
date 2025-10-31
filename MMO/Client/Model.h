#pragma once

#include "ModelReader.h"

class BoneBuffer;

class Model : public ModelReader
{
public:
	Model(string file);
	virtual ~Model();

	virtual void Render();

	void MakeBoneTransform();
	void SetBoneTransforms();

	int GetNodeByName(string name);
	UINT GetBoneSize() { return (UINT)bones.size(); }
	
	ModelMesh* GetModelMesh(int i = 0);
	Material* GetMaterial(string name);

protected:
	BoneBuffer* boneBuffer;

	map<int, Matrix> boneTransforms;
	Matrix* nodeTransforms;
};

