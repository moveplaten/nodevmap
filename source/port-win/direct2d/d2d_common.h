/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include "draw/draw.h"
#include "base/base.h"

#define SAFE_RELEASE_PTR(x) if (x) { x->Release(); x = nullptr; }

class D2dUtil
{
public:
    static D2dUtil* initD2dUtil(HWND hwnd)
    {
        if (!g_d2dutil)
        return new D2dUtil(hwnd);
    }

    static bool initD2dDevice(HWND hwnd);

    static D2dUtil* g_d2dutil;

    static float g_dpi_scale_X; //need test
    static float g_dpi_scale_Y; //need test

    HRESULT createDeviceTarget();
    double getDrawFPS() { return draw_fps; }
    ID2D1HwndRenderTarget* getRenderTarget() { return m_pRT; }

    static D2D1::ColorF toColorF(NvpColor color)
    {
        return D2D1::ColorF((float)color.Red / 255.0f,
                            (float)color.Green / 255.0f,
                            (float)color.Blue / 255.0f,
                            (float)color.Alpha / 255.0f);
    }

    static D2D1_RECT_F toRectF(const BaseRect& rect)
    {
        return D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);
    }

    void onResize(UINT width, UINT height)
    {
        D2D1_SIZE_U size;
        size.width = width;
        size.height = height;
        m_pRT->Resize(size);
        D2D1_SIZE_F client = m_pRT->GetSize();
        
        if (NvpLayout::getTopLayout())
        {
            BaseRect rect;
            rect.left = 0.0f;
            rect.top = 0.0f;
            rect.right = client.width;
            rect.bottom = client.height;
            NvpLayout::setBaseRect(NvpLayout::getTopLayout(), rect);

            rect.left = 0.0f;
            rect.top = 0.0f;
            rect.right = client.width;
            rect.bottom = client.height - 20.0f;
            NvpLayout::setBaseRect(NvpLayout::getTopNodeView(), rect);

            rect.left = 0.0f;
            rect.top = client.height - 20.0f;
            rect.right = client.width;
            rect.bottom = client.height;
            NvpLayout::setBaseRect(NvpLayout::getTopMenuStat(), rect);
        }
    }

    void onRender()
    {
        NvpDrawPort::beginDraw();
    }

    //void fillRect(const BaseRect& rect, COLORREF col, RecordOption opt = BeginEnd);

    D2dUtil(HWND hwnd) : m_hwnd(hwnd),
                         m_pD2DFactory(nullptr),
                         m_pRT(nullptr),
                         m_pDWriteFactory(nullptr),
                         m_pFontCollection(nullptr),
                         m_pDWriteFontFace(nullptr),
                         m_pDWriteFont(nullptr),
                         m_pWICImagingFactory(nullptr),
                         draw_fps(0)
    {
    }

    ~D2dUtil()
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

    IWICImagingFactory* m_pWICImagingFactory;

    double draw_fps;
    friend class NvpDrawPort;
};

class AutoSolidBrush
{
public:
    AutoSolidBrush(ID2D1RenderTarget* target) : pRT(target), brush(nullptr) {}

    ~AutoSolidBrush() { if (brush) brush->Release(); brush = nullptr; pRT = nullptr; }

    ID2D1SolidColorBrush* create(const D2D1_COLOR_F& color)
    {
        if (brush) return nullptr;
        
        HRESULT hr = pRT->CreateSolidColorBrush(color, &brush);
        if (FAILED(hr)) return nullptr;
        return brush;
    }

private:
    ID2D1SolidColorBrush* brush;
    ID2D1RenderTarget* pRT;
};
