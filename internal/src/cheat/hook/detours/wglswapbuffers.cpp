#include "pch.h"
#include "wglswapbuffers.hpp"
#include <cheat/module/module.hpp>

#include <ext/imgui/custom/fonts/font-icon.hpp>
#include <ext/imgui/custom/fonts/font-medium.hpp>
#include <ext/imgui/custom/fonts/font-regular.hpp>
#include <ext/imgui/custom/fonts/inter-regular.hpp>
#include <ext/imgui/custom/fonts/proggy-regular.hpp>
#include <ext/imgui/custom/fonts/notification-icon.hpp>

#include <src/cheat/gui/notifications/notifications.hpp>
#include <src/cheat/gui/spotify/spotify.hpp>


#include "sdk/internal/register/envregister.hpp"
//#include "ext/imgui/custom/fonts.hpp"

BOOL __stdcall WglSwapBuffersHook::DetourWglSwapBuffers(const HDC HandleContextDevice)
{
    static RECT OriginalClip;
    static ImGuiContext* CurrentImGuiContext = nullptr;

    static bool ContextInitialized = false;
    static bool JniContextInitialized = false;

    static JNIEnv* Env = nullptr;

    if (GetObjectType(HandleContextDevice) == 0) {
        return WglSwapBuffersOriginal(HandleContextDevice);
    }

    if (INSTANCE_DESTRUCT && !ContextInitialized)
    {
        if (JniContextInitialized)
        {
            EnvRegister::DestroyThreadEnv();
            ClassLoaderFactory.DeleteInstance();

            JvmPtrM->DetachCurrentThread();
            Env = nullptr;

            JniContextInitialized = false;
        }

        return WglSwapBuffersOriginal(HandleContextDevice);
    }

    if (!JniContextInitialized)
    {
        if (JvmPtrM->AttachCurrentThreadAsDaemon(reinterpret_cast<void**>(&Env), nullptr) != JNI_OK) {
            return WglSwapBuffersOriginal(HandleContextDevice);
        }

        EnvRegister::RegisterThreadEnv(Env);
        JniContextInitialized = true;
    }

    if (!ContextInitialized)
    {
        WINDOW_HANDLE = WindowFromDC(HandleContextDevice);

        IMGUI_CHECKVERSION();
        CurrentImGuiContext = ImGui::CreateContext();

        ImFontConfig CustomFont;
        CustomFont.FontDataOwnedByAtlas = false;

        ImGuiIO& IO = ImGui::GetIO();
        IO.Fonts->AddFontFromMemoryTTF(static_cast<void*>(poppins_regular), sizeof(poppins_regular), 14.F, &CustomFont);//0
        IO.Fonts->AddFontFromMemoryTTF(static_cast<void*>(poppins_medium), sizeof(poppins_medium), 14.F, &CustomFont);//1
        IO.Fonts->AddFontFromMemoryTTF(static_cast<void*>(icon_binary), sizeof(icon_binary), 14.F, &CustomFont);//2

        IO.Fonts->AddFontFromMemoryTTF((void*)poppins_regular, sizeof(poppins_regular), 10.F, &CustomFont);//3
        IO.Fonts->AddFontFromMemoryTTF((void*)poppins_regular, sizeof(poppins_regular), 13.F, &CustomFont);//4
        IO.Fonts->AddFontFromMemoryTTF((void*)poppins_regular, sizeof(poppins_regular), 20.F, &CustomFont);//5
        IO.Fonts->AddFontFromMemoryTTF((void*)inter_regular, sizeof(inter_regular), 20.F, &CustomFont);//6
        IO.Fonts->AddFontFromMemoryTTF((void*)proggy_regular, sizeof(proggy_regular), 16.F, &CustomFont);//7

        IO.Fonts->AddFontFromMemoryTTF((void*)poppins_medium, sizeof(poppins_medium), 18.F, &CustomFont);//8
        IO.Fonts->AddFontFromMemoryTTF((void*)poppins_medium, sizeof(poppins_medium), 15.F, &CustomFont);//9
        IO.Fonts->AddFontFromMemoryTTF((void*)notification_icon, sizeof(notification_icon), 22.F, &CustomFont);//10
        //IO.Fonts->AddFontFromMemoryTTF((void*)font_regular, sizeof(font_regular), 11.F, &CustomFont);//0
        //IO.Fonts->AddFontFromMemoryTTF((void*)font_regular, sizeof(font_regular), 26.F, &CustomFont);//1
        //IO.Fonts->AddFontFromMemoryTTF((void*)font_regular, sizeof(font_regular), 16.F, &CustomFont);//2
        //IO.Fonts->AddFontFromMemoryTTF((void*)font_semi, sizeof(font_semi), 17.F, &CustomFont);//3
        //IO.Fonts->AddFontFromMemoryTTF((void*)icon_binary, sizeof(icon_binary), 14.F, &CustomFont);//4

        ImGui_ImplWin32_Init(WINDOW_HANDLE);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsClassic();

        IO.IniFilename = NULL;

        WindowProcOriginal = SetWindowLongPtr(WINDOW_HANDLE, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcHooked));
        ContextInitialized = true;
    }

    if (ContextInitialized && WINDOW_HANDLE != WindowFromDC(HandleContextDevice) || INSTANCE_DESTRUCT)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();

        ImGui::DestroyContext(CurrentImGuiContext);

        SetWindowLongPtr(WINDOW_HANDLE, GWLP_WNDPROC, WindowProcOriginal);

        ContextInitialized = false;

        return WglSwapBuffersOriginal(HandleContextDevice);
    }

    static bool Clipped = true;
    if (UI_OPEN)
    {

        if (Clipped)
        {
            GetClipCursor(&OriginalClip);
            ClipCursor(nullptr);
            Clipped = false;
        }
    }
    else
    {
        if (!Clipped)
        {
            ClipCursor(&OriginalClip);
            Clipped = true;
        }

    }
    DWORD PreviousAffinity = GetWindowDisplayAffinity(WINDOW_HANDLE, nullptr);
   // SetWindowDisplayAffinity(WINDOW_HANDLE, WDA_EXCLUDEFROMCAPTURE);

    GLint Viewport[4];
    glGetIntegerv(GL_VIEWPORT, Viewport);
    glViewport(0, 0, Viewport[2], Viewport[3]);

    DisplayHeight = static_cast<float>(Viewport[3]);
    DisplayWidth = static_cast<float>(Viewport[2]);
    ImGui::GetIO().DisplaySize = ImVec2{ static_cast<float>(Viewport[2]) , static_cast<float>(Viewport[3]) };

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiStyle& Style = ImGui::GetStyle();

    Style.WindowShadowSize = 0.F;

    static float Alpha = 0.F;
    static ImVec2 PreviousWindowPos = ImGui::GetCursorPos();
    if (UI_OPEN)
    {
        ImGui::ShowDemoWindow();
        UserInterface::Render();
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0.F, 0.F), ImVec2{ static_cast<float>(Viewport[2]) , static_cast<float>(Viewport[3]) }, Color(15.F, 15.F, 15.F, 195.F * Alpha));
        Alpha = ImLerp(Alpha, 1.F, ImGui::GetIO().DeltaTime * 7.F);
    }
    
    if (!UI_OPEN) {
        Alpha = 0.F;
    }
       /* ImGui::SetNextWindowPos(ImVec2(0.F, 0.F));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
        {
            for (const auto Mod : ModuleManager::GetModules())
            {
                Env->PushLocalFrame(100);
                Mod->OnImGuiRender(Env);
                Mod->OnRender(Env);
                Env->PopLocalFrame(NULL);
            }

            for (const auto Mod : ModuleManager::GetThreadedModules())
            {
                Env->PushLocalFrame(100);
                Mod->OnImGuiRender(Env);
                Mod->OnRender(Env);
                Env->PopLocalFrame(NULL);
            }
        }
        ImGui::End(); */
    for (const auto& [Name, Mod] : ModuleManager::GetModules())
    {
        Env->PushLocalFrame(100);
        Mod->OnImGuiRender(Env);
        Mod->OnRender(Env);
        Env->PopLocalFrame(NULL);
    }

    for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
    {
        Env->PushLocalFrame(100);
        Mod->OnImGuiRender(Env);
        Mod->OnRender(Env);
        Env->PopLocalFrame(NULL);
    }

    NotificationHandlerPtr->Render();
    SpotifyHandlerPtr->Render();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return WglSwapBuffersOriginal(HandleContextDevice);
}

LRESULT WglSwapBuffersHook::WindowProcHooked(const HWND Handle, const UINT Message, const WPARAM WParam, const LPARAM LParam)
{
    if (InChat) {
        return CallWindowProcW(reinterpret_cast<WNDPROC>(WindowProcOriginal), Handle, Message, WParam, LParam);
    }

    auto PreviouslyHeld = [](const char* Module) -> bool*
    {
        static std::map<const char*, bool> BindMap;
        auto Bind = BindMap.find(Module);

        if (Bind == BindMap.end())
        {
            BindMap[Module] = false;
        }

        return &BindMap[Module];
    };

    if (Message != WM_KEYDOWN)
    {
        for (const auto& [Name, Mod] : ModuleManager::GetModules())
        {
            *PreviouslyHeld(Name.data()) = false;
        }

        for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
        {
            *PreviouslyHeld(Name.data()) = false;
        }
    }

    if (Message == WM_KEYDOWN && !UI_OPEN)
    {
        for (const auto& [Name, Mod] : ModuleManager::GetModules()) 
        {
            if (Mod->GetKey() == WParam)
            {
                if (*PreviouslyHeld(Name.data())) {
                    continue;
                }
                
                if (Name == "BackTrack")
                {
                    const auto Module = ModuleManager::GetModuleByName("Blink");
                    if (Module->GetState()) {
                        continue; //thrownotif
                    }
                }

                Mod->UpdateState();

                if (Mod->GetState()) {
                    NotificationHandlerPtr->RegisterNotification(Name, "module was toggled", 0, 2000);
                }
                else {
                    NotificationHandlerPtr->RegisterNotification(Name, "module was disabled", 1, 2000);
                }

                *PreviouslyHeld(Name.data()) = true;
               // std::cout << "Module state: " << Mod->GetState() << std::endl;
            }
            else {
                *PreviouslyHeld(Name.data()) = false;
            }
        }

        for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules()) 
        {
            if (Mod->GetKey() == WParam) 
            {
                if (*PreviouslyHeld(Name.data())) {
                    continue;
                }

                Mod->UpdateState();

                if (Mod->GetState()) {
                    NotificationHandlerPtr->RegisterNotification(Name, "module was toggled", 0, 2000);
                }
                else {
                    NotificationHandlerPtr->RegisterNotification(Name, "module was disabled", 1, 2000);
                }
               // std::cout << "Module state: " << Mod->GetState() << std::endl;
                *PreviouslyHeld(Name.data()) = true;
            }
            else {
                *PreviouslyHeld(Name.data()) = false;
            }
        }

    }
    if (Message == WM_KEYDOWN && WParam == UI_BIND)
    {
        UI_OPEN = !UI_OPEN;
    }
    if (UI_OPEN)
    {
        ImGui_ImplWin32_WndProcHandler(Handle, Message, WParam, LParam);
        return true;
    }

    return CallWindowProcW(reinterpret_cast<WNDPROC>(WindowProcOriginal), Handle, Message, WParam, LParam);
}
