#include "pch.h"
#include "Game.h"
#include "Device.h"
#include "Environment.h"
#include "ConstBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Timer.h"
#include "Font.h"
#include "Camera.h"
#include "SceneManager.h"
#include "MapEditorScene.h"
#include "InGameScene.h"
#include "Collider.h"
#include "ModelExportScene.h"
#include "NavMeshScene.h"

bool Game::s_exit = false;

Game::Game()
{
	Device::Get();
	Environment::Get();
	Input::Get();
	Timer::Get();
	Font::Get().Add();
	SceneManager::Get();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(Device::Get().GetDevice(), Device::Get().GetDeviceContext());

	SceneManager::Get().Add("ModelExport", new ModelExportScene());
	SceneManager::Get().Add("MapEditor", new MapEditorScene());
	SceneManager::Get().Add("NavMesh", new NavMeshScene(true));
	SceneManager::Get().Add("InGame", new InGameScene());
	SceneManager::Get().Play("InGame");
}

Game::~Game()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	Texture::Delete();
	Shader::Delete();
}

void Game::Update()
{
	Debug();
	Input::Get().Update();
	Timer::Get().Update();
	Environment::Get().GetMainCamera()->Update();
	SceneManager::Get().Update();
}

void Game::PreRender()
{
	SceneManager::Get().PreRender();
}

void Game::Render()
{
	Device::Get().SetRenderTarget();
	Environment::Get().Set();
	SceneManager::Get().Render();
}

void Game::PostRender()
{
	// Font
	Font::Get().GetDC()->BeginDraw();
	RenderFPS();
	Font::Get().GetDC()->EndDraw();

	// ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	ImGuizmo::SetOrthographic(false);
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	Environment::Get().PostRender();
	SceneManager::Get().PostRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Game::RenderFPS()
{
	wstring fps = L"FPS: " + to_wstring((int)Timer::Get().GetFPS());
	Font::Get().RenderText(fps, { 10,0,0 });
}

void Game::Debug()
{
	if (Input::Get().Down(VK_F5))
	{
		Collider::s_isColliderDraw = !Collider::s_isColliderDraw;
	}
}
