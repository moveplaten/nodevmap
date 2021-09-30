#include "base/base.h"
#include "direct2d/d2d_common.h"
#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    mousePt pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);
    pt.x = pt.x / D2dUtil::g_dpi_scale_X;
    pt.y = pt.y / D2dUtil::g_dpi_scale_Y;

    switch (message)
    {
    case WM_USER + 100:
    {
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

    case WM_RBUTTONDOWN:
    {
        baseMsg->hitTest(MouseRButtonDown, &pt);
    }
    break;

    case WM_RBUTTONUP:
    {
        baseMsg->hitTest(MouseRButtonUp, &pt);
    }
    break;

    case WM_SIZE:
    {
        if (D2dUtil::g_d2dutil)
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            D2dUtil::g_d2dutil->onResize(width, height);
        }
    }
    break;

    case WM_PAINT:
    {
        if (D2dUtil::g_d2dutil)
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            D2dUtil::g_d2dutil->onRender();
            EndPaint(hwnd, &ps);
        }
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
