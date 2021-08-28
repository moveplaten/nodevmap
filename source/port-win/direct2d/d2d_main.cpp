#include "d2d_common.h"

D2dUtil* D2dUtil::g_d2dutil = nullptr;

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

    return hr;
}

void D2dUtil::fillRect(const RECT& rect, COLORREF col)
{
    if (!g_d2dutil)
    {
        return;
    }
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

    m_pRT->BeginDraw();
    m_pRT->FillRectangle(recf, brush);
    hr = m_pRT->EndDraw();
    brush->Release();
}
