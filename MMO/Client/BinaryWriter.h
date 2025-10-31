#pragma once

class BinaryWriter
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryWriter(std::wstring filePath);
	BinaryWriter(std::string filePath);
	~BinaryWriter();

	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void String(std::string data);
	void Float4x4(Matrix data);

	void Byte(void* data, UINT dataSize);
};