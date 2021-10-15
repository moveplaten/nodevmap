#import <Cocoa/Cocoa.h>

#import "quartz/qtz_common.h"
#include "base/base.h"

////////////////////////////////////////////////////////////////////////////////

@implementation WndDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    NSLog(@"launch finish");
    auto draw_port = new QtzNvpDrawPort;
    nvpDraw->initDrawPort(draw_port);
    
    [self windowDidResize:notification]; //ensure size top_layout before MsgInit;
    
    baseMsg->hitTest(MsgInit, nullptr);
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (void)windowDidResize:(NSNotification *)notification
{
    NSRect wnd_rect = [g_main_wnd.contentView bounds];
    
    if (nvpBuild)
    {
        BaseRect rect;
        rect.left = 0.0f;
        rect.top = 0.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height;
        nvpDraw->Record(nvpBuild->g_top_layout, 0, nullptr, NoneDraw, &rect);

        rect.left = 0.0f;
        rect.top = 0.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height - 20.0f;
        nvpDraw->Record(nvpBuild->g_top_node_view, 0, nullptr, NoneDraw, &rect);
        
        rect.left = 0.0f;
        rect.top = wnd_rect.size.height - 20.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height;
        nvpDraw->Record(nvpBuild->g_top_menu_stat, 0, nullptr, NoneDraw, &rect);
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
    baseMsg->hitTest(MouseMove, &pt);
}

- (void)mouseDragged:(NSEvent *)event
{
    NSLog(@"mouse drag");
    mousePt pt = [self convertPt:event];
    baseMsg->hitTest(MouseMove_MouseLButtonDown, &pt);
}

- (void)mouseDown:(NSEvent *)event
{
    NSLog(@"LButton Down");
    mousePt pt = [self convertPt:event];
    baseMsg->hitTest(MouseLButtonDown, &pt);
}

- (void)mouseUp:(NSEvent *)event
{
    NSLog(@"LButton UP");
    mousePt pt = [self convertPt:event];
    baseMsg->hitTest(MouseLButtonUp, &pt);
}

- (void)rightMouseDown:(NSEvent *)event
{
    NSLog(@"RButton Down");
    mousePt pt = [self convertPt:event];
    baseMsg->hitTest(MouseRButtonDown, &pt);
}

- (void)rightMouseUp:(NSEvent *)event
{
    NSLog(@"RButton UP");
    mousePt pt = [self convertPt:event];
    baseMsg->hitTest(MouseRButtonUp, &pt);
}

- (void)drawRect:(NSRect)dirtyRect
{
    NSLog(@"Drawing");
    g_cg_ref = [[NSGraphicsContext currentContext] CGContext];
    
    if (g_ct_font_ref == nil)
    {
        g_ct_font_ref = CTFontCreateWithName((CFStringRef)@"Helvetica", 12, NULL);
    }
    
    if (nvpBuild)
    {
        auto top_elem = nvpBuild->g_top_layout;
        nvpDraw->drawAll(top_elem);
    }
}

@end
