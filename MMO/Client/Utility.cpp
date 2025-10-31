#include "pch.h"
#include "Utility.h"

string Utility::ToString(int value)
{
    return to_string(value);
}

string Utility::ToString(float value, UINT precision)
{
    string temp = to_string(value);
    string result{};
	result.assign(temp.begin(), temp.begin() + (precision - 1));
    return result;
}

string Utility::ToString(wstring value)
{
    string temp;
    temp.assign(value.begin(), value.end());
    return temp;
}

wstring Utility::ToWString(int value)
{
    return to_wstring(value);
}

wstring Utility::ToWString(float value, UINT precision)
{
    wstring temp = to_wstring(value);
    wstring result{};
    if (precision == 0)
        precision = (UINT)temp.size();
	result.assign(temp.begin(), temp.begin() + (precision - 1));
    return result;
}

wstring Utility::ToWString(string value)
{
    wstring temp;
    temp.assign(value.begin(), value.end());
    return temp;
}

void Utility::Replace(string* str, string comp, string rep)
{
    string temp = *str;

    size_t at = 0;
    while ((at = temp.find(comp, at)) != string::npos)
    {
        temp.replace(at, comp.length(), rep);
        at += rep.length();
    }

    *str = temp;
}

vector<string> Utility::SplitString(string origin, string tok)
{
    vector<string> result;

    size_t cutAt = 0;

    while ((cutAt = origin.find_first_of(tok)) != origin.npos)
    {
        if (cutAt > 0)
            result.emplace_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }

    return result;
}

wstring Utility::GetExtension(wstring path)
{
    size_t index = path.find_last_of('.');

    return path.substr(index + 1, path.length());
}

string Utility::GetDirectoryName(string path)
{
    Replace(&path, "\\", "/");

    size_t index = path.find_last_of('/');

    return path.substr(0, index + 1);
}

string Utility::GetFileName(string path)
{
    Replace(&path, "\\", "/");

    size_t index = path.find_last_of('/');

    return path.substr(index + 1, path.length());
}

string Utility::GetFileNameWithoutExtension(string path)
{
    string fileName = GetFileName(path);

    size_t index = fileName.find_last_of('.');

    return fileName.substr(0, index);
}

void Utility::CreateFolders(string path)
{
    vector<string> folders = SplitString(path, "/");

    string temp = "";
    for (string folder : folders)
    {
        temp += folder + "/";

        if (!ExistDirectory(temp))
            CreateDirectoryA(temp.c_str(), 0);
    }
}

bool Utility::ExistDirectory(string path)
{
    DWORD fileValue = GetFileAttributesA(path.c_str());

    BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES &&
        (fileValue & FILE_ATTRIBUTE_DIRECTORY));

    return temp == TRUE;
}

bool Utility::ExistFile(string path)
{
    DWORD fileValue = GetFileAttributesA(path.c_str());   

    return fileValue < 0xffffffff;
}

void Utility::Shuffle(vector<int>& arr)
{
	//for (int i = arr.size() - 1; i != 1; --i)
	//{
	//	int randIdx = Random(0, i);
 //       int temp = arr[i];
 //       arr[i] = arr[randIdx];
 //       arr[randIdx] = temp;
	//}
 //   
    return;
}