#include "pch.h"
#include "Utility.h"
#include "Environment.h"
#include "Camera.h"
#include "Control.h"

namespace Utility
{
    string ToString(int value)
    {
        return to_string(value);
    }

    string ToString(float value, UINT precision)
    {
        string temp = to_string(value);
        string result{};
        result.assign(temp.begin(), temp.begin() + (precision - 1));
        return result;
    }

    string ToString(wstring value)
    {
        string temp;
        temp.assign(value.begin(), value.end());
        return temp;
    }

    wstring ToWString(int value)
    {
        return to_wstring(value);
    }

    wstring ToWString(float value, UINT precision)
    {
        wstring temp = to_wstring(value);
        wstring result{};
        if (precision == 0)
            precision = (UINT)temp.size();
        result.assign(temp.begin(), temp.begin() + (precision - 1));
        return result;
    }

    wstring ToWString(string value)
    {
        wstring temp;
        temp.assign(value.begin(), value.end());
        return temp;
    }

    void Replace(string* str, string comp, string rep)
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

    vector<string> SplitString(string origin, string tok)
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

    wstring GetExtension(wstring path)
    {
        size_t index = path.find_last_of('.');

        return path.substr(index + 1, path.length());
    }

    string GetDirectoryName(string path)
    {
        Replace(&path, "\\", "/");

        size_t index = path.find_last_of('/');

        return path.substr(0, index + 1);
    }

    string GetFileName(string path)
    {
        Replace(&path, "\\", "/");

        size_t index = path.find_last_of('/');

        return path.substr(index + 1, path.length());
    }

    string GetFileNameWithoutExtension(string path)
    {
        string fileName = GetFileName(path);

        size_t index = fileName.find_last_of('.');

        return fileName.substr(0, index);
    }

    void CreateFolders(string path)
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

    bool ExistDirectory(string path)
    {
        DWORD fileValue = GetFileAttributesA(path.c_str());

        BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES &&
            (fileValue & FILE_ATTRIBUTE_DIRECTORY));

        return temp == TRUE;
    }

    bool ExistFile(string path)
    {
        DWORD fileValue = GetFileAttributesA(path.c_str());

        return fileValue < 0xffffffff;
    }

    Vector3 ClosestPointOnLineSegment(const Vector3& v1, const Vector3& v2, const Vector3& point)
    {
        Vector3 line = v2 - v1;
        float t = line.Dot(point - v1) / line.Dot(line);
        t = clamp(t, 0.0f, 1.0f);

        return v1 + t * line;
    }

    Vector3 ScreenToWorldHeightZero()
    {
        Ray ray = Environment::Get().GetMainCamera()->ScreenPointToRay(Control::Get().GetMouse());
        Vector3 S = ray.position;
        Vector3 D = ray.direction;
        float l = abs(S.y / D.y);
        Vector3 result = D * l + S;
        return result;
    }

    void Shuffle(vector<int>& arr)
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
}