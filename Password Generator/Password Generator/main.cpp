#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "LoveFont.h"
#include "dripicons.h"
#include "English.hpp"
#include "Finnish.hpp"
#include "wintoastlib.h"
#include "password_generator.hpp"
#include "clipboardxx.hpp"

#define S_p "Include Uppercase Letters(A - Z)                       "

// Data
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Style
void SetupImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.15f, 1.0f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.71f, 0.60f, 0.91f, 0.33f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.07f, 0.12f, 0.89f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.29f, 0.28f, 0.34f, 0.94f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.41f, 0.18f, 0.56f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.60f, 0.20f, 0.87f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.65f, 0.24f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.88f, 0.06f, 0.47f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.86f, 0.18f, 0.61f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.76f, 0.21f, 0.74f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.10f, 0.52f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.97f, 0.21f, 0.49f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.87f, 0.37f, 0.65f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.78f, 0.10f, 0.30f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.18f, 0.86f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.42f, 0.13f, 0.69f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.55f, 0.04f, 0.80f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.78f, 0.50f, 0.87f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.14f, 0.92f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.51f, 0.04f, 0.86f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.23f, 0.13f, 0.40f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.23f, 0.86f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.17f, 0.76f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


    style.WindowRounding = 12.0f;
    style.FrameRounding = 8.0f;
    style.ItemSpacing = ImVec2(10, 10);
    style.WindowPadding = ImVec2(20, 20);
    style.GrabRounding = 8.0f;
}

class CustomHandler : public WinToastLib::IWinToastHandler {
public:
    void toastActivated() const {
    }

    void toastActivated(int actionIndex) const {  }

    void toastDismissed(WinToastDismissalReason state) const {
    }

    void toastFailed() const {}
};

// Main code
int main(int, char**)
{
    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();   
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 50, 50, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    SetupImGuiStyle();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // window flags
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    window_flags |= ImGuiWindowFlags_NoResize;

    // Load Fonts
    ImFontConfig font_cfg, font_cfg_2;
    font_cfg.FontDataOwnedByAtlas = false;

    io.Fonts->AddFontFromMemoryTTF(LoveFont, LoveFont_size, 17.0f, &font_cfg); // basical font

    font_cfg_2.PixelSnapH = true;
    font_cfg_2.FontDataOwnedByAtlas = false;
    ImFont* myIconFont = io.Fonts->AddFontFromMemoryTTF(dripicons, dripicons_size, 19.f, &font_cfg_2);

    // Password tools
    int value = 32;
    bool include_symb = true;
    bool include_numbers = true;
    bool include_lower_letters = true;
    bool include_upper_letters = true;
    bool include_similar_letters = true;
    password_generator pass_gen;
    std::string password;

    // Language Tools
    std::unique_ptr<Language> lang = std::make_unique<English>();;

    // Copying tools
    clipboardxx::clipboard clipboard;

    // Notification tools
    WinToastLib::WinToast::instance()->setAppName(L"Password Generator");
    WinToastLib::WinToast::instance()->setAppUserModelId(L"Password Generator");
    WinToastLib::WinToast::instance()->initialize();
    WinToastLib::WinToastTemplate templ(WinToastLib::WinToastTemplate::Text02);
    templ.setTextField(L"Password copied!", WinToastLib::WinToastTemplate::FirstLine);
    templ.setTextField(L"The password has been successfully copied to your clipboard.", WinToastLib::WinToastTemplate::SecondLine);

    // Main loop
    bool done = false;
    bool isShow = true;

    while (!done && isShow)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();


        {
            ImGui::Begin("Password Generator", &isShow, window_flags);

            ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 360);
            ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 10);
            if (ImGui::BeginPopupContextItem("my popup"))
            {
                if (ImGui::Selectable("Enlish")) { lang = std::make_unique<English>(); }
                if (ImGui::Selectable("Finnish")) { lang = std::make_unique<Finnish>(); }
                templ.setTextField(lang->first_text(), WinToastLib::WinToastTemplate::FirstLine);
                templ.setTextField(lang->second_text(), WinToastLib::WinToastTemplate::SecondLine);
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::EndPopup();
            }

            if (ImGui::Button("Lang"))
            {
                ImGui::OpenPopup("my popup");
            }

            if (value > pass_gen.GetMaxSize())
            {
                value = pass_gen.GetMaxSize();
            }
            ImGui::SliderInt(lang->password_size(), &value, 0, pass_gen.GetMaxSize()); pass_gen.SetActualSize(value);
            ImGui::Separator();

            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImGui::Text(lang->include_symbs());
            cursorPos.x += ImGui::CalcTextSize(S_p).x + 20;
            ImGui::SetCursorPos(cursorPos);
            ImGui::Checkbox("##1", &include_symb);
            pass_gen.SetIncludeSymb(include_symb);


            cursorPos = ImGui::GetCursorPos();
            ImGui::Text(lang->include_nums());
            cursorPos.x += ImGui::CalcTextSize(S_p).x + 20;
            ImGui::SetCursorPos(cursorPos);
            ImGui::Checkbox("##2", &include_numbers);
            pass_gen.SetIncludeNumbers(include_numbers);

            cursorPos = ImGui::GetCursorPos();
            ImGui::Text(lang->include_lower_letters());
            cursorPos.x += ImGui::CalcTextSize(S_p).x + 20;
            ImGui::SetCursorPos(cursorPos);
            ImGui::Checkbox("##3", &include_lower_letters);
            pass_gen.SetIncludeLowerLetters(include_lower_letters);


            cursorPos = ImGui::GetCursorPos();
            ImGui::Text(lang->include_upper_letters());
            cursorPos.x += ImGui::CalcTextSize(S_p).x + 20;
            ImGui::SetCursorPos(cursorPos);
            ImGui::Checkbox("##4", &include_upper_letters);
            pass_gen.SetIncludeUpperLetters(include_upper_letters);


            cursorPos = ImGui::GetCursorPos();
            ImGui::Text(lang->include_similar_letters());
            cursorPos.x += ImGui::CalcTextSize(S_p).x + 20;
            ImGui::SetCursorPos(cursorPos);
            ImGui::Checkbox("##5", &include_similar_letters);
            pass_gen.SetIncludeSimilarLetters(include_similar_letters);
            pass_gen.update_show_password_p();
            if (ImGui::Button(lang->generate_password(), ImVec2(400, 50)))
            {
                password = pass_gen.CreatePassword().c_str();

            }

            ImGui::Spacing();
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 textSize = ImGui::CalcTextSize(password.c_str());
            cursorPos = ImGui::GetCursorPos();
            if (!password.empty())
            {
                cursorPos.x = windowSize.x - 35;
                cursorPos.y -= 10;
                ImGui::SetCursorPos(cursorPos);
                ImGui::PushFont(myIconFont);
                if (ImGui::Button("7", ImVec2(30, 30))) // copy button(font)
                {
                    clipboard << password;
                    WinToastLib::WinToast::instance()->showToast(templ, new CustomHandler());
                }
                ImGui::PopFont();
            }
            ImGui::SetCursorPosY(cursorPos.y);
            bool needsScrollbar = textSize.x > (windowSize.x - 70);
            if (needsScrollbar)
            {
                ImGui::BeginChild("ScrollableText", ImVec2(windowSize.x - 65, 35), false, ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
                ImGui::TextUnformatted(password.c_str());
                ImGui::EndChild();
            }
            else
            {
                ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
                ImGui::TextUnformatted(password.c_str());
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
