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
	Vector3 WorldToScreen(Vector3 worldPos);
	void LookAt(OUT float& rotationY, Vector3 direction);

	int Random(int min, int max);
	float Random(float min, float max);

	void Shuffle(vector<int>& arr);

	// Quaternion to euler angle 
	Vector3 ToEulerAngles(Quaternion q);

	// Timer
	bool TransitionTimer(OUT float& counter, OUT float& transitionTime, CallBack callBack);
	bool TransitionTimer(OUT float& counter, OUT float& transitionTime, CallBackParam callBack, int param);

	// Guizmo
	void TransformUsingGuizmo(Matrix& world, OUT Vector3& scale, OUT Vector3& rotation, OUT Vector3& position);
}