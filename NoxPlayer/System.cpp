#include "System.h"

System::System()
{
}

System::~System()
{
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    switch (umessage)
    {
        // 윈도우 종료를 확인합니다
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, umessage, wparam, lparam);
    }
}

bool System::Init(HINSTANCE& hinstance)
{
    m_hinstance = hinstance;

    if (!InitWindow())
        return false;

    m_Graphic = new Graphic;
    if (!m_Graphic->Init(m_hwnd))
        return false;

    m_Audio = new Audio;
    if (!m_Audio->Init())
        return false;

    return true;
}

bool System::InitWindow()
{
    m_applicationName = L"Blue-Arcive";

    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.lpfnWndProc = WndProc;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);

    posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

    m_hwnd = CreateWindowEx(NULL, m_applicationName, m_applicationName, WS_EX_OVERLAPPEDWINDOW,
        posX, posY, width, height, NULL, NULL, m_hinstance, NULL);

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
    SetFocus(m_hwnd);

    return true;
}

bool System::GetNoxPlayer()
{
    m_noxHwnd = FindWindow(NULL, L"NoxPlayer");
    if (m_noxHwnd == NULL)
        return false;
    return true;
}

void System::Run()
{
    MSG msg;

    m_Audio->Start();

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (!m_Graphic->NoxPlayerCapture(m_noxHwnd))
                break;
            m_Graphic->DrawBitmap();
            m_Graphic->Reset();
        }
    }

    m_Audio->End();

    m_Graphic->End(m_hwnd);
}

void System::Destroy()
{
    m_Graphic->Destroy();
    delete m_Graphic;
    m_Graphic = nullptr;

    m_Audio->Destroy();
    delete m_Audio;
    m_Audio = nullptr;
}