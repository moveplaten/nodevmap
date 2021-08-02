#include "base.h"
#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    mousePt pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    switch (message)
    {
    case WM_USER + 100:
    {
        baseMsg->setHwnd(hwnd);
        baseMsg->hitTest(MsgInit, &pt);
    }
    break;

    case WM_MOUSEMOVE:
    {
        baseMsg->hitTest(MouseMove, &pt);
        if (baseMsg->checkLeave() && BaseElement::g_before_leave_id != nullptr)
        {
            char temp[20];
            sprintf(temp, "%d", BaseElement::g_before_leave_id->getSelfID());
            OutputDebugStringA(temp);
            OutputDebugStringA("LEAVE\n");
            baseMsg->hitTest(MouseLeave, &pt);
        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
        baseMsg->hitTest(MouseLButtonDown, &pt);
    }
    break;

    case WM_LBUTTONUP:
    {
        baseMsg->hitTest(MouseLButtonUp, &pt);
    }
    break;

    case WM_CLOSE:
    {
        PostQuitMessage(0);
    }
    break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
