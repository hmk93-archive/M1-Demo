#include "pch.h"
#include "Model.h"
#include "GlobalBuffers.h"
#include "ModelType.h"

Model::Model(string file)
	: ModelReader(file)
{
	boneBuffer = new BoneBuffer();
	MakeBoneTransform();
	if (!bones.empty())
		typeBuffer->data.values[0] = 1;
}

Model::~Model()
{
	delete boneBuffer;
	delete[] nodeTransforms;
}

void Model::Render()
{
	SetBoneTransforms();
	boneBuffer->SetVSBuffer(3);
	MeshRender();
}

void Model::MakeBoneTransform()
{
	nodeTransforms = new Matrix[nodes.size()];
	UINT nodeIndex = 0;

	for (NodeData* node : nodes)
	{
		Matrix parent;
		int parentIndex = node->parent;

		if (parentIndex < 0)
			parent = Matrix::Identity;
		else
			parent = nodeTransforms[parentIndex];

		nodeTransforms[nodeIndex] = node->transform * parent;

		if (boneMap.count(node->name) > 0)
		{
			int boneIndex = boneMap[node->name];

			Matrix offset = bones[boneIndex]->offset;

			boneTransforms[boneIndex] = offset * nodeTransforms[nodeIndex];
		}

		nodeIndex++;
	}
}

void Model::SetBoneTransforms()
{
	for (auto bone : boneTransforms)
		boneBuffer->Add(bone.second, bone.first);
}

ModelMesh* Model::GetModelMesh(int i)
{
	if (meshes[i])
		return meshes[i];
	return nullptr;
}

Material* Model::GetMaterial(string name)
{
	if (materials[name])
		return materials[name];
	return nullptr;
}