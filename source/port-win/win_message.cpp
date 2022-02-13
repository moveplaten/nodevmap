/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base/nvp_base.h"
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

    NvpEventRef event;
    event.setWorldPt(pt);

    switch (message)
    {
    case WM_USER + 100:
    {
        NvpEvent::initAll(NvpLayout::Build()->getTop(), event);
    }
    break;

    case WM_MOUSEMOVE:
    {
        if ((DWORD)wParam && MK_LBUTTON)
        {
            event.setMouseType(NvpEventMouse::MouseLDrag);
            NvpEvent::handleEvent(NvpEvent::Mouse, event);
        }
        else
        {
            event.setMouseType(NvpEventMouse::MouseMove);
            NvpEvent::handleEvent(NvpEvent::Mouse, event);
        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
        event.setMouseType(NvpEventMouse::MouseLDown);
        NvpEvent::handleEvent(NvpEvent::Mouse, event);
    }
    break;

    case WM_LBUTTONUP:
    {
        event.setMouseType(NvpEventMouse::MouseLUp);
        NvpEvent::handleEvent(NvpEvent::Mouse, event);
    }
    break;

    case WM_RBUTTONDOWN:
    {
        event.setMouseType(NvpEventMouse::MouseRDown);
        NvpEvent::handleEvent(NvpEvent::Mouse, event);
    }
    break;

    case WM_RBUTTONUP:
    {
        event.setMouseType(NvpEventMouse::MouseRUp);
        NvpEvent::handleEvent(NvpEvent::Mouse, event);
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
