#include "d2d_common.h"
#include <stdio.h>

D2dUtil* D2dUtil::g_d2dutil = nullptr;
float D2dUtil::g_dpi_scale_X = 0.0f;
float D2dUtil::g_dpi_scale_Y = 0.0f;

bool initD2dDevice(HWND hwnd)
{
    HRESULT hr;
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        return false;
    }

    auto util = D2dUtil::initD2dUtil(hwnd);
    if (FAILED(util->createDeviceTarget()))
    {
        return false;
    }
    D2dUtil::g_d2dutil = util;
    return true;
}

HRESULT D2dUtil::createDeviceTarget()
{
    HRESULT hr;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(hr))
    {
        return hr;
    }

    RECT rc;
    GetClientRect(m_hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(
        rc.right - rc.left,
        rc.bottom - rc.top);

    // Create a Direct2D render target
    hr = m_pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &m_pRT);

    FLOAT dpiX, dpiY;
    m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
    g_dpi_scale_X = dpiX / 96.0f;
    g_dpi_scale_Y = dpiY / 96.0f;

    return hr;
}

#define MAX_QPC_CHANNEL 100

LONGLONG getQPCInterval(int channel)
{
    if (channel >= MAX_QPC_CHANNEL || channel < 0)
    {
        return -1;
    }
    LARGE_INTEGER QPC;
    QueryPerformanceCounter(&QPC);
    LONGLONG now = QPC.QuadPart;
    static LONGLONG prev[MAX_QPC_CHANNEL] = { 0 };
    LONGLONG QPCInterval = now - prev[channel];
    prev[channel] = now;
    return QPCInterval;
}

double getFPS(int channel)
{
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);
    double timeInterval = (double)getQPCInterval(channel) / (double)Frequency.QuadPart;
    double fps = (double)1.0 / timeInterval;
    return fps;
}

void D2dUtil::fillRect(const BaseRect& rect, COLORREF col, RecordOption opt)
{
    if (!g_d2dutil)
    {
        return;
    }

    auto mode = m_pRT->GetAntialiasMode();
    m_pRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED); //only render rect now;

    D2D1_RECT_F recf = D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);
    HRESULT hr;
    ID2D1SolidColorBrush* brush;
    auto colf = D2D1::ColorF(col);
    auto temp = colf.r;
    colf.r = colf.b;
    colf.b = temp;
    hr = m_pRT->CreateSolidColorBrush(colf, &brush);
    if (FAILED(hr))
    {
        return;
    }

    static bool can_fps_1, can_fps_2 = false;
    if ((opt == Begin || opt == BeginEnd) && opt != None)
    {
        m_pRT->BeginDraw();
        can_fps_1 = true;
    }

    m_pRT->FillRectangle(recf, brush);

    if ((opt == End || opt == BeginEnd) && opt != None)
    {
        hr = m_pRT->EndDraw();
        can_fps_2 = true;
    }

    brush->Release();

    if (can_fps_1 && can_fps_2)
    {
        draw_fps = getFPS(0);
        char tempc[30] = { 0 };
        sprintf(tempc, "draw fps : %f", draw_fps);
        SetWindowText(m_hwnd, tempc);
        can_fps_1 = can_fps_2 = false;
    }
}

void D2dDraw::doBegin()
{
    auto target = D2dUtil::g_d2dutil->m_pRT;
    target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    target->BeginDraw();
    target->Clear();
}

void D2dDraw::doDraw()
{
    auto target = D2dUtil::g_d2dutil->m_pRT;
    for (int i = 0; i < recs.size(); ++i)
    {
        auto rec = recs[i];
        auto col = cols[i];
        D2D1_RECT_F recf = D2D1::RectF(rec.left, rec.top, rec.right, rec.bottom);
        D2D1_ROUNDED_RECT round = { recf, 5.0f, 5.0f };
        auto color = RGB(col.Red, col.Green, col.Blue);
        auto colf = D2D1::ColorF(color);
        auto temp = colf.r;
        colf.r = colf.b;
        colf.b = temp;
        ID2D1SolidColorBrush* brush;
        HRESULT hr;
        hr = target->CreateSolidColorBrush(colf, &brush);
        if (FAILED(hr))
        {
            return;
        }
        if (elem->getSelfName() == "menu_bar" || elem->getSelfName() == "status_bar")
        {
            target->FillRectangle(recf, brush);
        }
        else
        {
            target->DrawRoundedRectangle(round, brush);
        }
        brush->Release();
    }
}

void D2dDraw::doEnd()
{
    auto target = D2dUtil::g_d2dutil->m_pRT;
    target->EndDraw();

    D2dUtil::g_d2dutil->draw_fps = getFPS(0);
    char tempc[30] = { 0 };
    sprintf(tempc, "draw fps : %f", D2dUtil::g_d2dutil->draw_fps);
    SetWindowText(D2dUtil::g_d2dutil->m_hwnd, tempc);
}
