#include "pch.h"
#include "Scene.h"

void Scene::CreateConsole()
{
	AllocConsole();
    FILE* fp = nullptr;
    freopen_s(&fp, "CONOUT$", "w", stdout); // printf 사용 가능
    freopen_s(&fp, "CONOUT$", "w", stderr); // fprintf(stderr)
    freopen_s(&fp, "CONIN$", "r", stdin);   // scanf 사용 가능
    SetConsoleTitle(L"MMO");
}
