/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#import "qtz_common.h"
#include "util/nvp_util.h"

NSWindow* g_main_wnd;
CGContextRef g_cg_ref;
CTFontRef g_ct_font_ref = nil;

static void toColorNS(NvpColor color)
{
    CGContextSetRGBFillColor(g_cg_ref, (CGFloat)color.Red / 255,
                                       (CGFloat)color.Green / 255,
                                       (CGFloat)color.Blue / 255,
                                       (CGFloat)color.Alpha / 255);

    CGContextSetRGBStrokeColor(g_cg_ref, (CGFloat)color.Red / 255,
                                         (CGFloat)color.Green / 255,
                                         (CGFloat)color.Blue / 255,
                                         (CGFloat)color.Alpha / 255);
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
    toColorNS(colo);
    
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
    toColorNS(colo);
    
    NSRect ns_rec = toRectNS(rect);
    
    CGContextFillRect(g_cg_ref, ns_rec);
}

void NvpDrawPort::frameRect(const BaseRect &rect, NvpColor colo)
{
    toColorNS(colo);
    
    NSRect ns_rec = toRectNS(rect);
    
    CGContextStrokeRectWithWidth(g_cg_ref, ns_rec, 1.0f);
}

void NvpDrawPort::drawOneLine(NvpXyCoord p1, NvpXyCoord p2, NvpColor colo)
{
    toColorNS(colo);
    
    CGPoint pt1, pt2;
    pt1.x = p1.x; pt1.y = p1.y;
    
    pt2.x = p2.x; pt2.y = p2.y;
    
    CGPoint points[] = { pt1, pt2 };
    CGContextStrokeLineSegments(g_cg_ref, points, 2);
}

void NvpDrawPort::outputImage(const char* file_name)
{
    //CGImageRef img_ref = CGBitmapContextCreateImage(g_cg_ref);
    //ColorSyncProfileRef profile_ref = ColorSyncProfileCreateWithDisplayID(0);
    //CGColorSpaceRef color_space = CGColorSpaceCreateWithPlatformColorSpace(profile_ref);
    const CGColorSpaceRef color_space = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    const uint32_t bitmap_info = kCGImageAlphaPremultipliedLast;
    const size_t bit_per = 8;
    const size_t width = 1000;
    const size_t height = 1000;
    CGContextRef cg_ref = CGBitmapContextCreate(NULL, width, height, bit_per, 0, color_space, bitmap_info);
    
    auto device_desc = [g_main_wnd deviceDescription];
    auto wnd_dpi = [[device_desc objectForKey:NSDeviceResolution] sizeValue];
    
    const CGFloat dpi_scale_x = wnd_dpi.width / 72.0f;
    const CGFloat dpi_scale_y = wnd_dpi.height / 72.0f;
    
    CGAffineTransform ctm = { 0 };
    ctm.a = dpi_scale_x; ctm.d = -dpi_scale_y; //flipped;
    CGContextConcatCTM(cg_ref, ctm);
    CGContextTranslateCTM(cg_ref, 0, (CGFloat)(height / ctm.d));
    ctm = CGContextGetCTM(cg_ref);
    //NSGraphicsContext* ns_graphic = [NSGraphicsContext graphicsContextWithCGContext:g_cg_ref flipped:YES];
    //[NSGraphicsContext setCurrentContext:ns_graphic];

    g_cg_ref = cg_ref;
    if (NvpLayout::getTopLayout())
    {
        auto top_elem = NvpLayout::getTopLayout();
        NvpDraw::drawAll(top_elem);
    }
    
    CGImageRef img_ref = CGBitmapContextCreateImage(cg_ref);
    NSString* file = [[NSString alloc] initWithUTF8String:file_name];
    NSBitmapImageRep* img_rep = [[NSBitmapImageRep alloc] initWithCGImage:img_ref];
    //[img_rep setPixelsHigh:1000];
    //[img_rep setPixelsWide:1000];
    NSDictionary* img_prop = [NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:1.0] forKey:NSImageCompressionFactor];
    NSData* png_data = [img_rep representationUsingType:NSBitmapImageFileTypePNG properties:img_prop];
    auto OK = [png_data writeToFile:file atomically:NO];
    [img_rep release];
    [file release];
    //[png_data release];
    CGColorSpaceRelease(color_space);
    CGImageRelease(img_ref);
    CGContextRelease(cg_ref);
}

void NvpDrawPort::outputPDF(const char* file_name)
{
    CFStringRef path = CFStringCreateWithCString(NULL, file_name, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, false);
    const CGFloat width = 1000;
    const CGFloat height = 1000;
    const CGRect rect = CGRectMake(0, 0, width, height);
    CGContextRef pdf_ref = CGPDFContextCreateWithURL(url, &rect, NULL);
    
    CFMutableDictionaryRef page_info = CFDictionaryCreateMutable(NULL,
                                                                 0,
                                                                 &kCFTypeDictionaryKeyCallBacks,
                                                                 &kCFTypeDictionaryValueCallBacks);
    CFDataRef box_data = CFDataCreate(NULL, (const UInt8*)&rect, sizeof(CGRect));
    CFDictionarySetValue(page_info, kCGPDFContextMediaBox, box_data);
    
    CGPDFContextBeginPage(pdf_ref, page_info);
    CGAffineTransform ctm = { 0 };
    ctm = CGContextGetCTM(pdf_ref);
    ctm.a = 1; ctm.d = -1; //flipped;
    CGContextConcatCTM(pdf_ref, ctm);
    CGContextTranslateCTM(pdf_ref, 0, (CGFloat)(height / ctm.d));
    ctm = CGContextGetCTM(pdf_ref);
    
    g_cg_ref = pdf_ref;
    if (NvpLayout::getTopLayout())
    {
        auto top_elem = NvpLayout::getTopLayout();
        NvpDraw::drawAll(top_elem);
    }
    CGPDFContextEndPage(pdf_ref);
    
    CFRelease(box_data);
    CFRelease(page_info);
    CGContextRelease(pdf_ref);
    CFRelease(url);
    CFRelease(path);
}
