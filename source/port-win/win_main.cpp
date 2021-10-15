#include "base/base.h"
#include "direct2d/d2d_common.h"

LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
#define MAINWNDCLASS TEXT("MainWindow")
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    NvpLevel::iterator iter;
    BaseElement b(0, "0", 0, 0, iter);
    nvpBuild->elemGen("test0", MsgNone, nullptr, nullptr);
    ElemStorage<100, int> e;

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = MAINWNDCLASS;
    wc.lpfnWndProc = (WNDPROC)MainProc;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hbrBackground = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW; //send paint message whatever window size change;
    RegisterClass(&wc);

    ///////////////////////////////////////////////
    //check and set dpi aware;
    bool dpi_aware = IsProcessDPIAware();
    bool result = SetProcessDPIAware();
    dpi_aware = IsProcessDPIAware();
    if (!result && !dpi_aware)
    {
        MessageBox(nullptr, "Set DPI Aware Failed", NULL, MB_OK);
        return false;
    }
    ///////////////////////////////////////////////

    int x, y, width, height;
    x = y = 100;
    width = height = 600;
    HWND hwndMain = CreateWindowEx(WS_EX_CONTROLPARENT | WS_EX_WINDOWEDGE | WS_EX_LEFT, MAINWNDCLASS, TEXT("nodevmap"),
        WS_OVERLAPPED | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN,
        x, y, width, height, NULL, NULL, hInstance, NULL);

    if (!initD2dDevice(hwndMain)) //give control to access to d2d;
    {
        MessageBox(nullptr, "Init Direct2d Failed", NULL, MB_OK);
        return false;
    }

    ShowWindow(hwndMain, SW_SHOW);
    UpdateWindow(hwndMain);
    SendMessage(hwndMain, WM_USER + 100, 0, 0);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
