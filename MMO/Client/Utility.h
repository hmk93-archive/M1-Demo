#pragma once

namespace Utility
{
	string ToString(int value);
	string ToString(float value, UINT precision = 0);
	string ToString(wstring value);
	wstring ToWString(int value);
	wstring ToWString(float value, UINT precision = 0);
	wstring ToWString(string value);

	void Replace(string* str, string comp, string rep);
	vector<string> SplitString(string origin, string tok);

	wstring GetExtension(wstring path);
	string GetDirectoryName(string path);
	string GetFileName(string path);
	string GetFileNameWithoutExtension(string path);

	void CreateFolders(string path);
	
	bool ExistDirectory(string path);
	bool ExistFile(string path);
	
	Vector3 ClosestPointOnLineSegment(const Vector3& v1, const Vector3& v2, const Vector3& point);
	Vector3 ScreenToWorldHeightZero();

	void Shuffle(vector<int>& arr);
}