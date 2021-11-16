#include "d2d_common.h"
#include "util/util.h"
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

    if (FAILED(hr))
    {
        return hr;
    }
    
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
    
    if (FAILED(hr))
    {
        return hr;
    }

    hr = m_pDWriteFactory->GetSystemFontCollection(&m_pFontCollection);
    
    if (FAILED(hr))
    {
        return hr;
    }
    
    UINT32 index;
    BOOL exist;
    hr = m_pFontCollection->FindFamilyName(L"Arial", &index, &exist);
    
    if (FAILED(hr))
    {
        return hr;
    }

    IDWriteFontFamily* p_fontfamily;
    hr = m_pFontCollection->GetFontFamily(index, &p_fontfamily);
    
    if (FAILED(hr))
    {
        return hr;
    }
    
    hr = p_fontfamily->GetFirstMatchingFont(DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, DWRITE_FONT_STYLE_NORMAL, &m_pDWriteFont);

    if (FAILED(hr))
    {
        return hr;
    }

    hr = m_pDWriteFont->CreateFontFace(&m_pDWriteFontFace);

    if (FAILED(hr))
    {
        return hr;
    }

    FLOAT dpiX, dpiY;
    m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
    g_dpi_scale_X = dpiX / 96.0f;
    g_dpi_scale_Y = dpiY / 96.0f;

    return hr;
}

void NvpDrawPort::beginDraw()
{
    auto target = D2dUtil::g_d2dutil->m_pRT;
    target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    
    target->BeginDraw();
    target->Clear();

    ////////////////////////////////////////////////////////////////////////////

    if (NvpLayout::getTopLayout())
    {
        auto top_elem = NvpLayout::getTopLayout();
        NvpDraw::drawAll(top_elem);
    }

    ////////////////////////////////////////////////////////////////////////////

    target->EndDraw();
    
    D2dUtil::g_d2dutil->draw_fps = nvpUtil->getFpsByChannel(0);
    char tempc[30] = { 0 };
    sprintf(tempc, "draw fps : %f", D2dUtil::g_d2dutil->draw_fps);
    SetWindowText(D2dUtil::g_d2dutil->m_hwnd, tempc);
}

void NvpDrawPort::drawTextFromLToR(NvpXyCoord start, const std::string& str,
    ptSize size_font, NvpColor colo)
{
    auto str_size = str.size();
    auto u32_str = nvpUtil->utf8_to_utf32(str);
    float font_size = size_font;
    
    DWRITE_GLYPH_RUN glyph_run;
    memset(&glyph_run, 0, sizeof(DWRITE_GLYPH_RUN));

    UINT16* glyph_index = new UINT16[str_size];
    FLOAT* glyph_advance = new FLOAT[str_size];

    glyph_run.fontFace = D2dUtil::g_d2dutil->m_pDWriteFontFace;
    glyph_run.fontEmSize = font_size;
    glyph_run.glyphCount = str_size;
    glyph_run.glyphIndices = glyph_index;
    glyph_run.glyphAdvances = glyph_advance;

    D2dUtil::g_d2dutil->m_pDWriteFontFace->GetGlyphIndices((UINT32 const*)u32_str.c_str(), str_size, glyph_index);

    DWRITE_GLYPH_METRICS* glyph_metric = new DWRITE_GLYPH_METRICS[str_size];

    D2dUtil::g_d2dutil->m_pDWriteFontFace->GetDesignGlyphMetrics(glyph_index, str_size, glyph_metric);

    DWRITE_FONT_METRICS font_metric;
    D2dUtil::g_d2dutil->m_pDWriteFontFace->GetMetrics(&font_metric);
    float scale = font_size / (float)font_metric.designUnitsPerEm;

    for (size_t i = 0; i < str_size; ++i)
    {
        glyph_advance[i] = (float)glyph_metric[i].advanceWidth * scale;
    }

    auto color = RGB(colo.Blue, colo.Green, colo.Red); //BGRA;
    auto colf = D2D1::ColorF(color);
    colf.a = 0.8f;
    ID2D1SolidColorBrush* brush;
    D2dUtil::g_d2dutil->m_pRT->CreateSolidColorBrush(colf, &brush);

    D2D1_POINT_2F point;
    point.x = start.x; point.y = start.y;
    D2dUtil::g_d2dutil->m_pRT->DrawGlyphRun(point, &glyph_run, brush);

    delete[](glyph_index);
    delete[](glyph_advance);
    delete[](glyph_metric);
    brush->Release();
}

void NvpDrawPort::fillRect(const BaseRect& rect, NvpColor colo)
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

void NvpDrawPort::frameRect(const BaseRect& rect, NvpColor colo)
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
