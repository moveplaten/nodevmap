#import <Cocoa/Cocoa.h>

#import "quartz/qtz_common.h"
#include "base/base.h"
#include "util/util.h"

////////////////////////////////////////////////////////////////////////////////

@implementation WndDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    NSLog(@"launch finish");
    
    [self windowDidResize:notification]; //ensure size top_layout before MsgInit;
    
    NvpEventParam param;
    NvpEvent::fromSysEvent(SysInit, param);
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (void)windowDidResize:(NSNotification *)notification
{
    NSRect wnd_rect = [g_main_wnd.contentView bounds];
    
    if (NvpLayout::getTopLayout())
    {
        BaseRect rect;
        rect.left = 0.0f;
        rect.top = 0.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height;
        NvpLayout::setBaseRect(NvpLayout::getTopLayout(), rect);

        rect.left = 0.0f;
        rect.top = 0.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height - 20.0f;
        NvpLayout::setBaseRect(NvpLayout::getTopNodeView(), rect);
        
        rect.left = 0.0f;
        rect.top = wnd_rect.size.height - 20.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height;
        NvpLayout::setBaseRect(NvpLayout::getTopMenuStat(), rect);
    }
}

@end

////////////////////////////////////////////////////////////////////////////////

@implementation MainView

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        [self addTrackingArea];
    }
    return self;
}

- (void)addTrackingArea
{
    NSTrackingAreaOptions options = (NSTrackingMouseMoved |
                                     NSTrackingInVisibleRect |
                                     NSTrackingActiveAlways); //same as Windows;
    
    NSTrackingArea* trackArea = [[NSTrackingArea alloc] initWithRect:NSZeroRect
                                                             options:options
                                                               owner:self
                                                            userInfo:nil];
    [self addTrackingArea:trackArea];
}

- (BOOL)isFlipped
{
    return YES; //set to left_top coord;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (mousePt)convertPt:(NSEvent *)event
{
    NSPoint ns_pt = [event locationInWindow];
    NSRect wnd_rect = [g_main_wnd.contentView frame];
    mousePt pt;
    pt.x = ns_pt.x;
    pt.y = wnd_rect.size.height - ns_pt.y;
    return pt;
}

- (void)mouseMoved:(NSEvent *)event
{
    //NSLog(@"mouse move");
    mousePt pt = [self convertPt:event];
    NvpEventParam param(pt);
    NvpEvent::fromSysEvent(MouseMove, param);
}

- (void)mouseDragged:(NSEvent *)event
{
    NSLog(@"mouse drag");
    mousePt pt = [self convertPt:event];
    NvpEventParam param(pt);
    NvpEvent::fromSysEvent(MouseLDrag, param);
}

- (void)mouseDown:(NSEvent *)event
{
    NSLog(@"LButton Down");
    mousePt pt = [self convertPt:event];
    NvpEventParam param(pt);
    NvpEvent::fromSysEvent(MouseLDown, param);
}

- (void)mouseUp:(NSEvent *)event
{
    NSLog(@"LButton UP");
    mousePt pt = [self convertPt:event];
    NvpEventParam param(pt);
    NvpEvent::fromSysEvent(MouseLUp, param);
}

- (void)rightMouseDown:(NSEvent *)event
{
    NSLog(@"RButton Down");
    mousePt pt = [self convertPt:event];
    NvpEventParam param(pt);
    NvpEvent::fromSysEvent(MouseRDown, param);
}

- (void)rightMouseUp:(NSEvent *)event
{
    NSLog(@"RButton UP");
    mousePt pt = [self convertPt:event];
    NvpEventParam param(pt);
    NvpEvent::fromSysEvent(MouseRUp, param);
}

- (void)drawRect:(NSRect)dirtyRect
{
    auto fps = NvpUtil::getFpsByChannel(0);
    NSLog(@"Drawing FPS %f", fps);
    g_cg_ref = [[NSGraphicsContext currentContext] CGContext];
    
    if (g_ct_font_ref == nil)
    {
        g_ct_font_ref = CTFontCreateWithName((CFStringRef)@"Helvetica", 12, NULL);
    }
    
    if (NvpLayout::getTopLayout())
    {
        auto top_elem = NvpLayout::getTopLayout();
        NvpDraw::drawAll(top_elem);
    }
}

@end
