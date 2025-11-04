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
#include "MenuScene.h"

bool Game::s_exit = false;
ImGuizmo::OPERATION g_guizmoOp = ImGuizmo::TRANSLATE;
ImGuizmo::MODE g_guizmoMode = ImGuizmo::WORLD;
bool g_useSnap = false;
float g_snap[3] = { 1.0f, 1.0f, 1.0f };

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
	SceneManager::Get().Add("InGame", new InGameScene());
	SceneManager::Get().Add("Menu", new MenuScene());
	SceneManager::Get().Add("NavMesh", new NavMeshScene(true)); // 카메라 주의: InGameScene 보다 먼저 로딩될 경우 Follow 카메라 동작하지 않음
	SceneManager::Get().Play("NavMesh");
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
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	UpdateGuizmo();
	
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
	if (Input::Get().Down(VK_ESCAPE))
	{
		Scene* scene = SceneManager::Get().GetCurrentScene();
		if (InGameScene* dynamic = dynamic_cast<InGameScene*>(scene))
		{
			SceneManager::Get().Delete("InGame");
			SceneManager::Get().Play("Menu");
		}
		else if (MapEditorScene* dynamic = dynamic_cast<MapEditorScene*>(scene))
		{
			SceneManager::Get().Delete("MapEditor");
			SceneManager::Get().Play("Menu");
		}
	}
}

void Game::UpdateGuizmo()
{
	if (ImGuizmo::IsUsing())
	{
		ImGui::Text("Using gizmo");
	}
	else
	{
		ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
	}

	if (ImGui::IsKeyPressed('T'))
		g_guizmoOp = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed('R'))
		g_guizmoOp = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed('Y'))
		g_guizmoOp = ImGuizmo::SCALE;

	if (ImGui::RadioButton("Translate", g_guizmoOp == ImGuizmo::TRANSLATE))
		g_guizmoOp = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", g_guizmoOp == ImGuizmo::ROTATE))
		g_guizmoOp = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", g_guizmoOp == ImGuizmo::SCALE))
		g_guizmoOp = ImGuizmo::SCALE;

	if (g_guizmoOp != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", g_guizmoMode == ImGuizmo::LOCAL))
			g_guizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", g_guizmoMode == ImGuizmo::WORLD))
			g_guizmoMode = ImGuizmo::WORLD;
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Y))
		g_useSnap = !g_useSnap;
	ImGui::Checkbox(" ", &g_useSnap);
	ImGui::SameLine();
	switch (g_guizmoOp)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &g_snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &g_snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &g_snap[0]);
		break;
	}
}
