#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>

#include "draw/draw.h"
#include "base/layout.h"

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
    }

    void onRender()
    {
        auto content = g_node_view->readOneElem(0);
        if (content->draw)
        {
            BaseRect rect;
            content->draw->Record(rect, { 0 });
        }
    }

    void fillRect(const BaseRect& rect, COLORREF col, RecordOption opt = BeginEnd);

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
    friend class D2dDraw;
};

class D2dDraw : public NvpDraw
{
public:
    virtual void doBegin() override;
    virtual void doDraw() override;
    virtual void doEnd() override;

    D2dDraw::D2dDraw()
    {
    }

    D2dDraw::~D2dDraw()
    {
    }

private:

};
