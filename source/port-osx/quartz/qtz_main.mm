#import "qtz_common.h"
#include "util/util.h"

NSWindow* g_main_wnd;
CGContextRef g_cg_ref;
CTFontRef g_ct_font_ref = nil;

void NvpDrawPort::beginDraw()
{
    [g_main_wnd.contentView setNeedsDisplay:YES];
}

void NvpDrawPort::drawTextFromLToR(NvpXyCoord start, const std::string& str,
                                   ptSize font_size, NvpColor colo)
{
    NSColor* ns_col = [NSColor colorWithCalibratedRed:(CGFloat)colo.Red/255
                                                green:(CGFloat)colo.Green/255
                                                 blue:(CGFloat)colo.Blue/255
                                                alpha:(CGFloat)colo.Alpha/255];
    [ns_col set];
    
    CGContextSetTextPosition(g_cg_ref, start.x, start.y);
    CGAffineTransform font_ctm;
    font_ctm = CGContextGetTextMatrix(g_cg_ref);
    CGFloat font_scale = font_size / 12.0f;
    font_ctm.d = -font_scale; //flipped;
    font_ctm.a = font_scale;
    CGContextSetTextMatrix(g_cg_ref, font_ctm);
    
    auto str_size = str.size();
    auto u16_str = nvpUtil->utf8_to_utf16(str);
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
    NSColor* ns_col = [NSColor colorWithCalibratedRed:(CGFloat)colo.Red/255
                                                green:(CGFloat)colo.Green/255
                                                 blue:(CGFloat)colo.Blue/255
                                                alpha:(CGFloat)colo.Alpha/255];
    [ns_col set];
    
    NSRect ns_rec;
    ns_rec.origin.x = rect.left;
    ns_rec.origin.y = rect.top;
    ns_rec.size.width = rect.right - rect.left;
    ns_rec.size.height = rect.bottom - rect.top;
    
    CGContextFillRect(g_cg_ref, ns_rec);
}

void NvpDrawPort::frameRect(const BaseRect &rect, NvpColor colo)
{
    NSColor* ns_col = [NSColor colorWithCalibratedRed:(CGFloat)colo.Red/255
                                                green:(CGFloat)colo.Green/255
                                                 blue:(CGFloat)colo.Blue/255
                                                alpha:(CGFloat)colo.Alpha/255];
    [ns_col set];
    
    NSRect ns_rec;
    ns_rec.origin.x = rect.left;
    ns_rec.origin.y = rect.top;
    ns_rec.size.width = rect.right - rect.left;
    ns_rec.size.height = rect.bottom - rect.top;
    
    CGContextStrokeRectWithWidth(g_cg_ref, ns_rec, 1.0f);
}
