#import "qtz_common.h"
#include "util/util.h"

NSWindow* g_main_wnd;
CGContextRef g_cg_ref;
CTFontRef g_ct_font_ref = nil;

static NSColor* toColorNS(NvpColor color)
{
    return [NSColor colorWithCalibratedRed:(CGFloat)color.Red / 255
                                     green:(CGFloat)color.Green / 255
                                      blue:(CGFloat)color.Blue / 255
                                     alpha:(CGFloat)color.Alpha / 255];
}

static NSRect toRectNS(const BaseRect& rect)
{
    NSRect ns_rect;
    ns_rect.origin.x = rect.left;
    ns_rect.origin.y = rect.top;
    ns_rect.size.width = rect.right - rect.left;
    ns_rect.size.height = rect.bottom - rect.top;
    return ns_rect;
}

void NvpDrawPort::beginDraw()
{
    [g_main_wnd.contentView setNeedsDisplay:YES];
}

void NvpDrawPort::drawTextFromLToR(NvpXyCoord start, const std::string& str,
                                   ptSize font_size, NvpColor colo)
{
    NSColor* ns_col = toColorNS(colo);
    [ns_col set];
    
    CGContextSetTextPosition(g_cg_ref, start.x, start.y);
    CGAffineTransform font_ctm;
    font_ctm = CGContextGetTextMatrix(g_cg_ref);
    CGFloat font_scale = font_size / 12.0f;
    font_ctm.d = -font_scale; //flipped;
    font_ctm.a = font_scale;
    CGContextSetTextMatrix(g_cg_ref, font_ctm);
    
    auto str_size = str.size();
    auto u16_str = NvpUtil::utf8_to_utf16(str);
    CGGlyph* glyphs = new CGGlyph[str_size];
    CGRect* boundings = new CGRect[str_size];
    CGPoint* points = new CGPoint[str_size];
    memset(points, 0, str_size * sizeof(CGPoint));
    
    CTFontGetGlyphsForCharacters(g_ct_font_ref, (const UniChar*)u16_str.c_str(), glyphs, str_size);
    //auto box = CTFontGetBoundingBox(g_ct_font_ref);
    CTFontGetOpticalBoundsForGlyphs(g_ct_font_ref, glyphs, boundings, str_size, NULL);
    
    for (size_t i = 0; i < str_size - 1; ++i)
    {
        points[i + 1].x = points[i].x + boundings[i].size.width;
    }
    
    CTFontDrawGlyphs(g_ct_font_ref, glyphs, points, str_size, g_cg_ref);
    
    delete[](glyphs);
    delete[](boundings);
    delete[](points);
}

void NvpDrawPort::fillRect(const BaseRect &rect, NvpColor colo)
{
    NSColor* ns_col = toColorNS(colo);
    [ns_col set];
    
    NSRect ns_rec = toRectNS(rect);
    
    CGContextFillRect(g_cg_ref, ns_rec);
}

void NvpDrawPort::frameRect(const BaseRect &rect, NvpColor colo)
{
    NSColor* ns_col = toColorNS(colo);
    [ns_col set];
    
    NSRect ns_rec = toRectNS(rect);
    
    CGContextStrokeRectWithWidth(g_cg_ref, ns_rec, 1.0f);
}

void NvpDrawPort::drawOneLine(NvpXyCoord p1, NvpXyCoord p2, NvpColor colo)
{
    NSColor* ns_col = toColorNS(colo);
    [ns_col set];
    
    CGPoint pt1, pt2;
    pt1.x = p1.x; pt1.y = p1.y;
    
    pt2.x = p2.x; pt2.y = p2.y;
    
    CGPoint points[] = { pt1, pt2 };
    CGContextStrokeLineSegments(g_cg_ref, points, 1);
}
