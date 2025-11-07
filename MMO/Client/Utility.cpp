#include "pch.h"
#include "Utility.h"
#include "Environment.h"
#include "Camera.h"
#include "Input.h"
#include "Timer.h"

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
        Ray ray = Environment::Get().GetMainCamera()->ScreenPointToRay(Input::Get().GetMouse());
        Vector3 S = ray.position;
        Vector3 D = ray.direction;
        float l = abs(S.y / D.y);
        Vector3 result = D * l + S;
        return result;
    }

    Vector3 WorldToScreen(Vector3 worldPos)
    {
        Vector3 screenPos;

        screenPos = XMVector3TransformCoord(worldPos, Environment::Get().GetMainCamera()->GetView());
        screenPos = XMVector3TransformCoord(screenPos, Environment::Get().GetProjection());

        screenPos.x = (screenPos.x + 1.0f) * 0.5f;
        screenPos.y = (screenPos.y + 1.0f) * 0.5f;
        
        screenPos.x *= g_screenWidth;
        screenPos.y *= g_screenHeight;

        return screenPos;
    }

    void LookAt(OUT float& rotationY, Vector3 direction)
    {
        float f1 = atan2f(direction.x, direction.z);
        float f2 = atan2f(Vector3::Forward.x, Vector3::Forward.z);
        float result = f1 - f2;
        if (result < 0)
            result += XM_2PI;
        rotationY = result;
    }

    int Random(int min, int max)
    {
        return rand() % (max - min) + min;
    }

    float Random(float min, float max)
    {
        float normal = rand() / (float)RAND_MAX;
        return min + (max - min) * normal;
    }

    void Shuffle(vector<int>& arr)
    {
        for (int i = arr.size() - 1; i != 1; --i)
        {
        	int randIdx = Random(0, i);
            int temp = arr[i];
            arr[i] = arr[randIdx];
            arr[randIdx] = temp;
        }
    }

    Vector3 ToEulerAngles(Quaternion q)
    {
        Vector3 angles;

        // roll (x-axis rotation)
        double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
        double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
        angles.x = (float)std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
        double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
        angles.y = 2.f * (float)std::atan2(sinp, cosp) - 3.14159f / 2;

        // yaw (z-axis rotation)
        double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
        double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
        angles.z = (float)std::atan2(siny_cosp, cosy_cosp);

        return angles;
    }

    bool TransitionTimer(OUT float& counter, OUT float& transitionTime, CallBack callBack)
    {
        counter += Timer::Get().GetElapsedTime();
        if (counter > transitionTime)
        {
            counter = 0;
            callBack();
            return true;
        }
        return false;
    }

    bool TransitionTimer(OUT float& counter, OUT float& transitionTime, CallBackParam callBack, int param)
    {
        counter += Timer::Get().GetElapsedTime();
        if (counter > transitionTime)
        {
            counter = 0;
            callBack(param);
            return true;
        }
        return false;
    }

    void TransformUsingGuizmo(Matrix& world, OUT Vector3& scale, OUT Vector3& rotation, OUT Vector3& position)
    {
        float view[16], proj[16];
        memcpy(view, &Environment::Get().GetMainCamera()->GetView(), sizeof(view));
        memcpy(proj, &Environment::Get().GetProjection(), sizeof(proj));
        ImGuizmo::Manipulate(
            view,
            proj,
            g_guizmoOp,
            g_guizmoMode,
            (float*)&world,
            nullptr,
            g_useSnap ? &g_snap[0] : nullptr);
        Quaternion q;
        world.Decompose(scale, q, position);
        rotation = ToEulerAngles(q);
    }
}