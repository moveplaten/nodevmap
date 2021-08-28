#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>

enum DrawOption
{
    Begin,
    End,
    BeginEnd,
    None,
};

bool initD2dDevice(HWND hwnd);

class D2dUtil
{
public:
    static D2dUtil* initD2dUtil(HWND hwnd)
    {
        if (!g_d2dutil)
        return new D2dUtil(hwnd);
    }

    static D2dUtil* g_d2dutil;

    HRESULT createDeviceTarget();
    double getDrawFPS() { return draw_fps; }

    void fillRect(const RECT& rect, COLORREF col, DrawOption opt = BeginEnd);

    D2dUtil::D2dUtil(HWND hwnd):m_hwnd(hwnd),
                                m_pD2DFactory(nullptr),
                                m_pRT(nullptr),
                                draw_fps(0)
    {
    }

    D2dUtil::~D2dUtil()
    {
    }

private:
    HWND m_hwnd;
    ID2D1Factory* m_pD2DFactory;
    ID2D1HwndRenderTarget* m_pRT;

    double draw_fps;
};
