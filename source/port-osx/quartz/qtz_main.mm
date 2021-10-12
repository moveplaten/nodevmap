#import "qtz_common.h"

NSWindow* g_main_wnd;
CGContextRef g_cg_ref;

void QtzNvpDrawPort::beginDraw()
{
    [g_main_wnd.contentView setNeedsDisplay:YES];
}

void QtzNvpDrawPort::fillRect(const BaseRect &rect, NvpColor colo)
{
    NSColor* ns_col = [NSColor colorWithCalibratedRed:(CGFloat)colo.Red/255
                                                green:(CGFloat)colo.Green/255
                                                 blue:(CGFloat)colo.Blue/255
                                                alpha:(CGFloat)0.8];
    [ns_col set];
    
    NSRect ns_rec;
    ns_rec.origin.x = rect.left;
    ns_rec.origin.y = rect.top;
    ns_rec.size.width = rect.right - rect.left;
    ns_rec.size.height = rect.bottom - rect.top;
    
    CGContextFillRect(g_cg_ref, ns_rec);
}

void QtzNvpDrawPort::frameRect(const BaseRect &rect, NvpColor colo)
{
    NSColor* ns_col = [NSColor colorWithCalibratedRed:(CGFloat)colo.Red/255
                                                green:(CGFloat)colo.Green/255
                                                 blue:(CGFloat)colo.Blue/255
                                                alpha:(CGFloat)0.8];
    [ns_col set];
    
    NSRect ns_rec;
    ns_rec.origin.x = rect.left;
    ns_rec.origin.y = rect.top;
    ns_rec.size.width = rect.right - rect.left;
    ns_rec.size.height = rect.bottom - rect.top;
    
    CGContextStrokeRectWithWidth(g_cg_ref, ns_rec, 1.0f);
}
