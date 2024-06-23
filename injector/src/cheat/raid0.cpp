#include "pch.h"
#include "raid0.hpp"
#include <ext/imgui/imgui_impl_dx9.h>
#include <ext/imgui/imgui_impl_win32.h>

#include <src/cheat/fonts/font-icon.hpp>
#include <src/cheat/fonts/font-medium.hpp>
#include <src/cheat/fonts/font-regular.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);

long __stdcall WindowProcess( HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter)) {
		return true;
	}

	switch (message)
	{
		case WM_SIZE:
		{
			const auto Device = InstancePtr->GetDevice();

			if (Device && wideParameter != SIZE_MINIMIZED)
			{
				auto PresentParameters = InstancePtr->GetPresentParameters();

				PresentParameters.BackBufferWidth = LOWORD(longParameter);
				PresentParameters.BackBufferHeight = HIWORD(longParameter);

				InstancePtr->ResetDevice();
			}
		}
		return 0;

		case WM_SYSCOMMAND: 
		{
			if ((wideParameter & 0xfff0) == SC_KEYMENU) { // Disable ALT application menu
				return 0;
			}
		}
		break;

		case WM_DESTROY: 
		{
			PostQuitMessage(0);
		}
		return 0;

		case WM_LBUTTONDOWN: 
		{
			InstancePtr->SetPosition(MAKEPOINTS(longParameter)); // set click points
		}
		return 0;

		case WM_MOUSEMOVE: 
		{
			if (wideParameter == MK_LBUTTON)
			{
				const auto Points = MAKEPOINTS(longParameter);
				auto Rect = ::RECT{ };

				const auto WIDTH = InstancePtr->GetWidth();
				const auto Window = InstancePtr->GetWindow();
				auto Position = InstancePtr->GetPosition();

				GetWindowRect(Window, &Rect);

				Rect.left += Points.x - Position.x;
				Rect.top += Points.y - Position.y;

				if (Position.x >= 0 && Position.x <= WIDTH && Position.y >= 0 && Position.y <= 19) {
					SetWindowPos(Window, HWND_TOPMOST, Rect.left, Rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
				}
			}

		}
		return 0;
	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void Raid0::CreateHWindow() noexcept
{
	const auto RandomClassName = RandomNumberGenerator().GenerateString(10);

	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_CLASSDC;
	WindowClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WindowProcess);
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandleA(0);
	WindowClass.hIcon = 0;
	WindowClass.hCursor = 0;
	WindowClass.hbrBackground = 0;
	WindowClass.lpszMenuName = 0;
	WindowClass.lpszClassName = RandomClassName.data();
	WindowClass.hIconSm = 0;

	RegisterClassEx(&WindowClass);

	Window = CreateWindowEx(0, RandomClassName.data(), RandomNumberGenerator().GenerateString(10).data(), WS_POPUP, 100, 100, WIDTH, HEIGHT, 0, 0, WindowClass.hInstance, 0);

	ShowWindow(Window, SW_SHOWDEFAULT);
	UpdateWindow(Window);
}

void Raid0::DestroyHWindow() noexcept
{
	DestroyWindow(Window);
	UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
}

void Raid0::ResizeWindow(int Width, int Height) noexcept
{
	WIDTH = Width;
	HEIGHT = Height;

	SetWindowPos(Window, NULL, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER);
}

bool Raid0::CreateDevice() noexcept
{
	Direct3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!Direct3D) {
		return false;
	}

	ZeroMemory(&PresentParameters, sizeof(PresentParameters));

	PresentParameters.Windowed = TRUE;
	PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	PresentParameters.EnableAutoDepthStencil = TRUE;
	PresentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (Direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &PresentParameters, &Device) < 0) {
		return false;
	}

	return true;
}

void Raid0::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	if (Device->Reset(&PresentParameters) == D3DERR_INVALIDCALL) {
		IM_ASSERT(0);
	}

	ImGui_ImplDX9_CreateDeviceObjects();
}

void Raid0::DestroyDevice() noexcept
{
	if (Device)
	{
		Device->Release();
		Device = nullptr;
	}

	if (Direct3D)
	{
		Direct3D->Release();
		Direct3D = nullptr;
	}
}

void Raid0::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ::ImGui::GetIO();

	IO.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX9_Init(Device);

	IO.Fonts->AddFontFromMemoryTTF((poppins_regular), sizeof(poppins_regular), 15.F, 0);
	IO.Fonts->AddFontFromMemoryTTF((poppins_regular), sizeof(poppins_regular), 13.F);
	IO.Fonts->AddFontFromMemoryTTF((poppins_medium), sizeof(poppins_medium), 36.F);
}

void Raid0::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Raid0::BeginRender() noexcept
{
	MSG Message;
	while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);

		if (Message.message == WM_QUIT)
		{
			IsRunning = !IsRunning;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Raid0::EndRender() noexcept
{
	ImGui::EndFrame();

	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 0.f, 0);

	if (Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		Device->EndScene();
	}

	const auto Result = Device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (Result == D3DERR_DEVICELOST && Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ResetDevice();
	}
}

std::unique_ptr<Raid0> InstancePtr;