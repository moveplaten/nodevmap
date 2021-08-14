#include "base/base.h"
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
        if ((DWORD)wParam && MK_LBUTTON)
        {
            baseMsg->hitTest(MouseMove_MouseLButtonDown, &pt);
        }
        else
        {
            baseMsg->hitTest(MouseMove, &pt);
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
