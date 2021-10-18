#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#include "draw/draw.h"
#include "base/base.h"

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

    static float g_dpi_scale_X; //need test
    static float g_dpi_scale_Y; //need test

    HRESULT createDeviceTarget();
    double getDrawFPS() { return draw_fps; }
    auto getRenderTarget() { return m_pRT; }

    void onResize(UINT width, UINT height)
    {
        D2D1_SIZE_U size;
        size.width = width;
        size.height = height;
        m_pRT->Resize(size);
        D2D1_SIZE_F client = m_pRT->GetSize();
        
        if (nvpBuild)
        {
            BaseRect rect;
            rect.left = 0.0f;
            rect.top = 0.0f;
            rect.right = client.width;
            rect.bottom = client.height;
            nvpDraw->Record(nvpBuild->g_top_layout, 0, nullptr, NoneDraw, &rect);

            rect.left = 0.0f;
            rect.top = 0.0f;
            rect.right = client.width;
            rect.bottom = client.height - 20.0f;
            nvpDraw->Record(nvpBuild->g_top_node_view, 0, nullptr, NoneDraw, &rect);

            rect.left = 0.0f;
            rect.top = client.height - 20.0f;
            rect.right = client.width;
            rect.bottom = client.height;
            nvpDraw->Record(nvpBuild->g_top_menu_stat, 0, nullptr, NoneDraw, &rect);
        }
    }

    void onRender()
    {
        auto draw = nvpDraw->getDrawPort();
        if (draw)
        {
            draw->beginDraw();
        }
    }

    //void fillRect(const BaseRect& rect, COLORREF col, RecordOption opt = BeginEnd);

    D2dUtil::D2dUtil(HWND hwnd):m_hwnd(hwnd),
                                m_pD2DFactory(nullptr),
                                m_pRT(nullptr),
                                m_pDWriteFactory(nullptr),
                                m_pFontCollection(nullptr),
                                m_pDWriteFontFace(nullptr),
                                m_pDWriteFont(nullptr),
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

    IDWriteFactory* m_pDWriteFactory;
    IDWriteFontCollection* m_pFontCollection;
    
    IDWriteFontFace* m_pDWriteFontFace;
    IDWriteFont* m_pDWriteFont;

    double draw_fps;
    friend class NvpDrawPort;
};
