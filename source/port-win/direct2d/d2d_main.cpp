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

void D2dNvpDrawPort::beginDraw()
{
    auto target = D2dUtil::g_d2dutil->m_pRT;
    target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    
    target->BeginDraw();
    target->Clear();

    ////////////////////////////////////////////////////////////////////////////

    if (nvpBuild)
    {
        auto top_elem = nvpBuild->g_top_layout;
        nvpDraw->drawAll(top_elem);
    }

    ////////////////////////////////////////////////////////////////////////////

    target->EndDraw();
    
    D2dUtil::g_d2dutil->draw_fps = getFPS(0);
    char tempc[30] = { 0 };
    sprintf(tempc, "draw fps : %f", D2dUtil::g_d2dutil->draw_fps);
    SetWindowText(D2dUtil::g_d2dutil->m_hwnd, tempc);
}

void D2dNvpDrawPort::fillRect(const BaseRect& rect, NvpColor colo)
{
    auto target = D2dUtil::g_d2dutil->m_pRT;

    D2D1_RECT_F recf = D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);

    auto color = RGB(colo.Blue, colo.Green, colo.Red); //BGRA;
    auto colf = D2D1::ColorF(color);
    colf.a = 0.8f;
    
    ID2D1SolidColorBrush* brush;
    HRESULT hr;
    hr = target->CreateSolidColorBrush(colf, &brush);
    if (FAILED(hr))
    {
        return;
    }

    target->FillRectangle(recf, brush);

    brush->Release();
}

void D2dNvpDrawPort::frameRect(const BaseRect& rect, NvpColor colo)
{
    auto target = D2dUtil::g_d2dutil->m_pRT;

    D2D1_RECT_F recf = D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);

    auto color = RGB(colo.Blue, colo.Green, colo.Red); //BGRA;
    auto colf = D2D1::ColorF(color);
    colf.a = 0.8f;

    ID2D1SolidColorBrush* brush;
    HRESULT hr;
    hr = target->CreateSolidColorBrush(colf, &brush);
    if (FAILED(hr))
    {
        return;
    }

    target->DrawRectangle(recf, brush);

    brush->Release();
}
