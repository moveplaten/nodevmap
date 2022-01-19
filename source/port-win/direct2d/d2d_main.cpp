/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "d2d_common.h"
#include "util/nvp_util.h"
#include <stdio.h>

D2dUtil* D2dUtil::g_d2dutil = nullptr;
float D2dUtil::g_dpi_scale_X = 0.0f;
float D2dUtil::g_dpi_scale_Y = 0.0f;

bool D2dUtil::initD2dDevice(HWND hwnd)
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
    
    D2dUtil::g_d2dutil->draw_fps = NvpUtil::getFpsByChannel(0);
    char tempc[30] = { 0 };
    sprintf(tempc, "draw fps : %f", D2dUtil::g_d2dutil->draw_fps);
    SetWindowText(D2dUtil::g_d2dutil->m_hwnd, tempc);
}

void NvpDrawPort::drawTextFromLToR(NvpXyCoord start, const std::string& str,
    ptSize size_font, NvpColor colo)
{
    auto str_size = str.size();
    auto u32_str = NvpUtil::utf8_to_utf32(str);
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

    auto colf = D2dUtil::toColorF(colo);

    AutoSolidBrush auto_brush(D2dUtil::g_d2dutil->m_pRT);
    auto brush = auto_brush.create(colf);
    if (!brush) return;

    D2D1_POINT_2F point;
    point.x = start.x; point.y = start.y;
    D2dUtil::g_d2dutil->m_pRT->DrawGlyphRun(point, &glyph_run, brush);

    delete[](glyph_index);
    delete[](glyph_advance);
    delete[](glyph_metric);
}

void NvpDrawPort::fillRect(const BaseRect& rect, NvpColor colo)
{
    auto target = D2dUtil::g_d2dutil->m_pRT;

    auto recf = D2dUtil::toRectF(rect);

    auto colf = D2dUtil::toColorF(colo);

    AutoSolidBrush auto_brush(target);
    auto brush = auto_brush.create(colf);
    if (!brush) return;
    
    target->FillRectangle(recf, brush);
}

void NvpDrawPort::frameRect(const BaseRect& rect, NvpColor colo)
{
    auto target = D2dUtil::g_d2dutil->m_pRT;

    auto recf = D2dUtil::toRectF(rect);

    auto colf = D2dUtil::toColorF(colo);

    AutoSolidBrush auto_brush(target);
    auto brush = auto_brush.create(colf);
    if (!brush) return;

    target->DrawRectangle(recf, brush);
}

void NvpDrawPort::drawOneLine(NvpXyCoord p1, NvpXyCoord p2, NvpColor colo)
{
    auto target = D2dUtil::g_d2dutil->m_pRT;
    
    auto pt1 = D2D1::Point2F(p1.x, p1.y);
    auto pt2 = D2D1::Point2F(p2.x, p2.y);

    auto colf = D2dUtil::toColorF(colo);

    AutoSolidBrush auto_brush(target);
    auto brush = auto_brush.create(colf);
    if (!brush) return;

    target->DrawLine(pt1, pt2, brush);
}

void NvpDrawPort::outputImage(const char* file_name)
{
    IWICBitmap* p_WICBitmap = nullptr;
    ID2D1RenderTarget* p_WicRT = nullptr;
    IWICStream* p_WICStream = nullptr;
    IWICBitmapEncoder* p_WICBitmapEncoder = nullptr;
    IWICBitmapFrameEncode* p_WICBitmapFrameEncode = nullptr;

    do
    {
        HRESULT hr;
        if (!D2dUtil::g_d2dutil->m_pWICImagingFactory)
        {
            hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&D2dUtil::g_d2dutil->m_pWICImagingFactory));

            if (FAILED(hr)) break;
        }

        hr = D2dUtil::g_d2dutil->m_pWICImagingFactory->CreateBitmap(1000, 1000, //origin size;
            GUID_WICPixelFormat32bppBGR, WICBitmapCacheOnLoad, &p_WICBitmap);

        if (FAILED(hr)) break;

        hr = D2dUtil::g_d2dutil->m_pD2DFactory->CreateWicBitmapRenderTarget(p_WICBitmap,
            D2D1::RenderTargetProperties(), &p_WicRT);

        if (FAILED(hr)) break;

        ////////////////////////////////////////////////////////////////////////////////

        p_WicRT->SetDpi(D2dUtil::g_dpi_scale_X * 96, D2dUtil::g_dpi_scale_Y * 96);
        p_WicRT->BeginDraw();

        ID2D1HwndRenderTarget* tempRT;
        tempRT = D2dUtil::g_d2dutil->m_pRT;
        D2dUtil::g_d2dutil->m_pRT = reinterpret_cast<ID2D1HwndRenderTarget*>(p_WicRT);

        if (NvpLayout::getTopLayout())
        {
            auto top_elem = NvpLayout::getTopLayout();
            NvpDraw::drawAll(top_elem);
        }

        p_WicRT->EndDraw();
        D2dUtil::g_d2dutil->m_pRT = tempRT;

        ////////////////////////////////////////////////////////////////////////////////

        hr = D2dUtil::g_d2dutil->m_pWICImagingFactory->CreateStream(&p_WICStream);

        if (FAILED(hr)) break;

        std::string path_u8(file_name);
        auto path_u16 = NvpUtil::utf8_to_utf16(path_u8);

        hr = p_WICStream->InitializeFromFilename((LPCWSTR)path_u16.c_str(), GENERIC_WRITE);

        if (FAILED(hr)) break;

        hr = D2dUtil::g_d2dutil->m_pWICImagingFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &p_WICBitmapEncoder);

        if (FAILED(hr)) break;

        hr = p_WICBitmapEncoder->Initialize(p_WICStream, WICBitmapEncoderNoCache);

        if (FAILED(hr)) break;

        hr = p_WICBitmapEncoder->CreateNewFrame(&p_WICBitmapFrameEncode, NULL);

        if (FAILED(hr)) break;

        hr = p_WICBitmapFrameEncode->Initialize(NULL);

        if (FAILED(hr)) break;

        hr = p_WICBitmapFrameEncode->SetSize(1000, 1000); //clip size for output;

        if (FAILED(hr)) break;

        WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;
        hr = p_WICBitmapFrameEncode->SetPixelFormat(&format);

        if (FAILED(hr)) break;

        hr = p_WICBitmapFrameEncode->WriteSource(p_WICBitmap, NULL);

        if (FAILED(hr)) break;

        hr = p_WICBitmapFrameEncode->Commit();

        if (FAILED(hr)) break;

        hr = p_WICBitmapEncoder->Commit();

        if (FAILED(hr)) break;
    } while (0);

    SAFE_RELEASE_PTR(p_WICBitmap);
    SAFE_RELEASE_PTR(p_WicRT);
    SAFE_RELEASE_PTR(p_WICStream);
    SAFE_RELEASE_PTR(p_WICBitmapEncoder);
    SAFE_RELEASE_PTR(p_WICBitmapFrameEncode);
}
