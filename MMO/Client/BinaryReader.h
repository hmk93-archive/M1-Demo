#pragma once

class BinaryReader
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryReader(std::wstring filePath);
	BinaryReader(std::string filePath);
	~BinaryReader();

	int Int();
	UINT UInt();
	float Float();
	std::string String();
	Matrix Float4x4();

	void Byte(void** data, UINT dataSize);
};