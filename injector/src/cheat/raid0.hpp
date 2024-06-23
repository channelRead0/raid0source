#pragma once
#include "pch.h"
#include "../globals/utilities/rng.hpp"

class Raid0
{
private:
	int WIDTH = 505;
	int HEIGHT = 386;

	bool RenderStages = false;

	// when this changes, exit threads
	// and close menu :)
	bool IsRunning = true;

	// winapi window vars
	HWND Window = nullptr;
	WNDCLASSEX WindowClass = { };

	// points for window movement
	POINTS Position = { };

	// direct x state vars
	PDIRECT3D9 Direct3D = nullptr;
	LPDIRECT3DDEVICE9 Device = nullptr;
	D3DPRESENT_PARAMETERS PresentParameters = { };
	
public:
	void CreateHWindow() noexcept;
	void DestroyHWindow() noexcept;
	void ResizeWindow(int Width, int Height) noexcept;

	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;

    int GetWidth() const {
        return WIDTH;
    }
    void SetWidth(int width) {
        WIDTH = width;
    }

    int GetHeight() const {
        return HEIGHT;
    }
    void SetHeight(int height) {
        HEIGHT = height;
    }

    bool GetRenderStages() const {
        return RenderStages;
    }
    void SetRenderStages(bool renderStages) {
        RenderStages = renderStages;
    }

    bool GetIsRunning() const {
        return IsRunning;
    }
    void SetIsRunning(bool isRunning) {
        IsRunning = isRunning;
    }

    HWND GetWindow() const {
        return Window;
    }
    void SetWindow(HWND window) {
        Window = window;
    }

    WNDCLASSEX GetWindowClass() const {
        return WindowClass;
    }
    void SetWindowClass(const WNDCLASSEX& windowClass) {
        WindowClass = windowClass;
    }

    POINTS GetPosition() const {
        return Position;
    }
    void SetPosition(const POINTS& position) {
        Position = position;
    }

    PDIRECT3D9 GetDirect3D() const {
        return Direct3D;
    }
    void SetDirect3D(PDIRECT3D9 direct3D) {
        Direct3D = direct3D;
    }

    LPDIRECT3DDEVICE9 GetDevice() const {
        return Device;
    }
    void SetDevice(LPDIRECT3DDEVICE9 device) {
        Device = device;
    }

    D3DPRESENT_PARAMETERS GetPresentParameters() const {
        return PresentParameters;
    }
    void SetPresentParameters(const D3DPRESENT_PARAMETERS& presentParameters) {
        PresentParameters = presentParameters;
    }
};

extern std::unique_ptr<Raid0> InstancePtr;