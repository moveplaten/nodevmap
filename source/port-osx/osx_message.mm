/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#import <Cocoa/Cocoa.h>

#import "quartz/qtz_common.h"
#include "base/base.h"
#include "util/nvp_util.h"

////////////////////////////////////////////////////////////////////////////////

@implementation WndDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    NSLog(@"launch finish");
    
    [self windowDidResize:notification]; //ensure size top_layout before MsgInit;
    
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
    
    if (NvpLayout::Build()->getTop())
    {
        NvpRect rect;
        rect.left = 0.0f;
        rect.top = 0.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height;
        NvpLayout::setLayoutRect(NvpLayout::Build()->getTop(), rect);

        rect.left = 0.0f;
        rect.top = 0.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height - 20.0f;
        NvpLayout::setLayoutRect(NvpLayout::Build()->getNodeView(), rect);
        
        rect.left = 0.0f;
        rect.top = wnd_rect.size.height - 20.0f;
        rect.right = wnd_rect.size.width;
        rect.bottom = wnd_rect.size.height;
        NvpLayout::setLayoutRect(NvpLayout::Build()->getMenuStat(), rect);
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

- (NvpPoint)convertPt:(NSEvent *)event
{
    NSPoint ns_pt = [event locationInWindow];
    NSRect wnd_rect = [g_main_wnd.contentView frame];
    NvpPoint pt;
    pt.x = ns_pt.x;
    pt.y = wnd_rect.size.height - ns_pt.y;
    return pt;
}

- (void)mouseMoved:(NSEvent *)event
{
    //NSLog(@"mouse move");
    NvpPoint pt = [self convertPt:event];
    NvpEventRef event_ref;
    event_ref.setWorldPt(pt);
    event_ref.setMouseType(NvpEventMouse::MouseMove);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)mouseDragged:(NSEvent *)event
{
    NSLog(@"mouse drag");
    NvpPoint pt = [self convertPt:event];
    NvpEventRef event_ref;
    event_ref.setWorldPt(pt);
    event_ref.setMouseType(NvpEventMouse::MouseLDrag);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)mouseDown:(NSEvent *)event
{
    NSLog(@"LButton Down");
    NvpPoint pt = [self convertPt:event];
    NvpEventRef event_ref;
    event_ref.setWorldPt(pt);
    event_ref.setMouseType(NvpEventMouse::MouseLDown);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)mouseUp:(NSEvent *)event
{
    NSLog(@"LButton UP");
    NvpPoint pt = [self convertPt:event];
    NvpEventRef event_ref;
    event_ref.setWorldPt(pt);
    event_ref.setMouseType(NvpEventMouse::MouseLUp);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)rightMouseDown:(NSEvent *)event
{
    NSLog(@"RButton Down");
    NvpPoint pt = [self convertPt:event];
    NvpEventRef event_ref;
    event_ref.setWorldPt(pt);
    event_ref.setMouseType(NvpEventMouse::MouseRDown);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
}

- (void)rightMouseUp:(NSEvent *)event
{
    NSLog(@"RButton UP");
    NvpPoint pt = [self convertPt:event];
    NvpEventRef event_ref;
    event_ref.setWorldPt(pt);
    event_ref.setMouseType(NvpEventMouse::MouseRUp);
    NvpEvent::handleEvent(NvpEvent::Mouse, event_ref);
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
        auto top_elem = NvpLayout::Build()->getTop();
        NvpDraw::drawAll(top_elem);
    }
}

@end
