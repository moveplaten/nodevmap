/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#import <Cocoa/Cocoa.h>

#import "quartz/qtz_common.h"
#include "base/nvp_base.h"
#include "util/nvp_util.h"

////////////////////////////////////////////////////////////////////////////////

@implementation WndDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    NSLog(@"launch finish");
    
    [self windowDidResize:notification]; //ensure size top_layout before Init;
    
    NvpEventRef event;
    NvpEvent::initAll(NvpLayout::Build()->getTop(), event);
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (void)windowDidResize:(NSNotification *)notification
{
    NSRect wnd_rect = [g_main_wnd.contentView bounds];
    NvpEventView::handleView(wnd_rect.size.width, wnd_rect.size.height);
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

- (void)convertPt:(NSEvent *)event ref:(NvpEventRef &)event_ref
{
    NSPoint ns_pt = [event locationInWindow];
    NSRect wnd_rect = [g_main_wnd.contentView frame];
    NvpPoint pt;
    pt.x = ns_pt.x;
    pt.y = wnd_rect.size.height - ns_pt.y;
    event_ref.setWindowPt(pt);
    NvpPoint PT = NvpEventView::convertPt(pt);
    event_ref.setWorldPt(PT);
}

- (void)mouseMoved:(NSEvent *)event
{
    //NSLog(@"mouse move");
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    event_ref.setMouseType(NvpEventMouse::MouseMove);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)mouseDragged:(NSEvent *)event
{
    NSLog(@"mouse drag");
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    event_ref.setMouseType(NvpEventMouse::MouseLDrag);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)otherMouseDragged:(NSEvent *)event
{
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    event_ref.setMouseType(NvpEventMouse::WheelDrag);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)otherMouseDown:(NSEvent *)event
{
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    event_ref.setMouseType(NvpEventMouse::WheelPush);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)mouseDown:(NSEvent *)event
{
    NSLog(@"LButton Down");
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    event_ref.setMouseType(NvpEventMouse::MouseLDown);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)mouseUp:(NSEvent *)event
{
    NSLog(@"LButton UP");
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    event_ref.setMouseType(NvpEventMouse::MouseLUp);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)rightMouseDown:(NSEvent *)event
{
    NSLog(@"RButton Down");
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    event_ref.setMouseType(NvpEventMouse::MouseRDown);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)rightMouseUp:(NSEvent *)event
{
    NSLog(@"RButton UP");
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    event_ref.setMouseType(NvpEventMouse::MouseRUp);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)scrollWheel:(NSEvent *)event
{
    NvpEventRef event_ref;
    [self convertPt:event ref:event_ref];
    
    auto wheel = [event scrollingDeltaY];
    if (wheel < 0)
    {
        event_ref.setMouseType(NvpEventMouse::WheelUp);
        NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
    }
    else if (wheel > 0)
    {
        event_ref.setMouseType(NvpEventMouse::WheelDown);
        NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
    }
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
    
    if (NvpLayout::Build()->getTop())
    {
        auto device_desc = [g_main_wnd deviceDescription];
        auto wnd_dpi = [[device_desc objectForKey:NSDeviceResolution] sizeValue];
        
        const CGFloat dpi_scale_x = wnd_dpi.width / 72.0f;
        const CGFloat dpi_scale_y = wnd_dpi.height / 72.0f;
        NvpEventView::setDpiScale(dpi_scale_x, dpi_scale_y);
        
        NvpDrawPort::setDrawMatrix(NvpEventView::event_view->current_mtx);
        auto top_elem = NvpLayout::Build()->getTop();
        NvpDraw::drawAll(top_elem);
    }
}

@end
