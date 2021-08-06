#include "base.h"

class ActInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect;
        rect.left = rect.top = 10;
        rect.right = rect.bottom = 50;
        base->setRect(&rect);
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}ActInit;

class ActMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(200, 200, 200));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}ActMouseMove;

class ActMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(150, 150, 150));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}ActMouseLeave;

class ActMouseLButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {

    }
}ActMouseLButtonDown;

class ActMouseLButtonUp : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {

    }
}ActMouseLButtonUp;

////////////////////////////////////////////////////////////////////////
class Act2Init : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect;
        rect.left = 50;
        rect.right = 90;
        rect.top = 10;
        rect.bottom = 50;
        base->setRect(&rect);
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(0, 255, 0));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}Act2Init;

class Act2MouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(0, 200, 200));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}Act2MouseMove;

class Act2MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(0, 150, 150));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}Act2MouseLeave;
/////////////////////////////////////////////////////////////////////////


LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
#define MAINWNDCLASS TEXT("MainWindow")
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    VirtualElement v1;
    v1.base->linkMsg(MsgInit, &ActInit);
    v1.base->linkMsg(MouseMove, &ActMouseMove);
    v1.base->linkMsg(MouseLeave, &ActMouseLeave);
    v1.base->linkMsg(MouseLButtonDown, &ActMouseLButtonDown);
    v1.base->linkMsg(MouseLButtonUp, &ActMouseLButtonUp);
    VirtualElement v2;
    v2.base->linkMsg(MsgInit, &Act2Init);
    v2.base->linkMsg(MouseMove, &Act2MouseMove);
    v2.base->linkMsg(MouseLeave, &Act2MouseLeave);
    VirtualElement v3;
    VirtualElement v4;
    //mousePt pt;
    //pt.x = 11; pt.y = 33;
    //baseMsg->hitTest(MouseMove, &pt);
    //baseMsg->hitTest(MouseLButtonDown, &pt);
    //baseMsg->hitTest(MouseLButtonUp, &pt);
    BaseElement b;
    const baseRect* brec = BaseElement::getElementByID(0)->getRect();
    BaseElement::getElementByID(1)->setRect(brec);

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = MAINWNDCLASS;
    wc.lpfnWndProc = (WNDPROC)MainProc;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    RegisterClass(&wc);

    int x, y, width, height;
    x = y = 100;
    width = height = 600;
    HWND hwndMain = CreateWindowEx(WS_EX_CONTROLPARENT | WS_EX_WINDOWEDGE | WS_EX_LEFT, MAINWNDCLASS, TEXT("nodevmap"),
        WS_OVERLAPPED | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN,
        x, y, width, height, NULL, NULL, hInstance, NULL);

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
