/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base/base.h"
#include "direct2d/d2d_common.h"
#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    NvpPoint pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);
    pt.x = pt.x / D2dUtil::g_dpi_scale_X;
    pt.y = pt.y / D2dUtil::g_dpi_scale_Y;

    NvpEventParam param(pt);

    switch (message)
    {
    case WM_USER + 100:
    {
        NvpEvent::fromSysEvent(SysInit, param);
    }
    break;

    case WM_MOUSEMOVE:
    {
        if ((DWORD)wParam && MK_LBUTTON)
        {
            NvpEvent::fromSysEvent(MouseLDrag, param);
        }
        else
        {
            NvpEvent::fromSysEvent(MouseMove, param);
        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
        NvpEvent::fromSysEvent(MouseLDown, param);
    }
    break;

    case WM_LBUTTONUP:
    {
        NvpEvent::fromSysEvent(MouseLUp, param);
    }
    break;

    case WM_RBUTTONDOWN:
    {
        NvpEvent::fromSysEvent(MouseRDown, param);
    }
    break;

    case WM_RBUTTONUP:
    {
        NvpEvent::fromSysEvent(MouseRUp, param);
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
