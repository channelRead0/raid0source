#include "pch.h"
#include <cheat/raid0.hpp>
#include <cheat/authentication/authentication.hpp>


inline FILE* ConsoleFilePointer;


void AllocateConsole()
{
	if (GetConsoleWindow()) {
		return;
	}

	if (!AllocConsole()) {
		return;
	}

	const HWND ConsoleHandle{ GetConsoleWindow() };
	constexpr RECT ConsoleBounds{ 904 + 219, 420 + 420 + 69 };
	RECT WindowRect{};

	SetConsoleTitle("molly - debug");

	GetWindowRect(ConsoleHandle, &WindowRect);
	MoveWindow(ConsoleHandle, WindowRect.left, WindowRect.top, ConsoleBounds.left, ConsoleBounds.top, true);

	SetWindowLong(ConsoleHandle, GWL_STYLE, GetWindowLong(ConsoleHandle, GWL_STYLE) | WS_BORDER);
	SetWindowLong(ConsoleHandle, GWL_EXSTYLE, GetWindowLong(ConsoleHandle, GWL_EXSTYLE) | WS_EX_LAYERED);

	SetLayeredWindowAttributes(ConsoleHandle, 0, 230, 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);


	try {
		freopen_s(&ConsoleFilePointer, "CONOUT$", "w", stdout);
	}
	catch (const std::exception& Exception) {
		std::cerr << "Caught an exception while assigning stdout: " << Exception.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Caught an unknown exception while assigning stdout." << std::endl;
	}

	try {
		freopen_s(&ConsoleFilePointer, "CONOUT$", "w", stderr);
	}
	catch (const std::exception& Exception) {
		std::cerr << "Caught an exception while assigning stderr: " << Exception.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Caught an unknown exception while assigning stderr." << std::endl;
	}
}

void DeAllocateConsole()
{
	if (!GetConsoleWindow()) {
		return;
	}

	if (ConsoleFilePointer != nullptr)
	{
		fclose(ConsoleFilePointer);
		ConsoleFilePointer = nullptr; // Reset file pointer to nullptr after closing
	}

	ShowWindow(GetConsoleWindow(), SW_HIDE);
	FreeConsole();
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	AllocateConsole();

	InstancePtr = std::make_unique<Raid0>();

	InstancePtr->CreateHWindow();
	InstancePtr->CreateDevice();
	InstancePtr->CreateImGui();

	////TEST
	auto Auth = new Authentication("RACECARNIGGA");
	Auth->Authenticate();
	while (InstancePtr->GetIsRunning())
	{
		InstancePtr->BeginRender();
		InstancePtr->Render();
		InstancePtr->EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	InstancePtr->DestroyImGui();
	InstancePtr->DestroyDevice();
	InstancePtr->DestroyHWindow();

	return EXIT_SUCCESS;
}