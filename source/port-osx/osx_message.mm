#import <Cocoa/Cocoa.h>
#include "base/base.h"

@interface WndDelegate : NSObject <NSWindowDelegate, NSApplicationDelegate>

@end

@interface MainView : NSView

@end

NSWindow* g_main_wnd;

////////////////////////////////////////////////////////////////////////////////

@implementation WndDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    NSLog(@"launch finish");
    baseMsg->hitTest(MsgInit, nullptr);
    elemGen("node_1", MsgNone, nullptr);
    elemDel("top_layout", g_top_layout);
}

- (BOOL)windowShouldClose:(NSWindow *)sender
{
    [NSApp terminate:g_main_wnd];
    return FALSE;
}

@end

////////////////////////////////////////////////////////////////////////////////

@implementation MainView
{
    NSTrackingArea* trackArea;
}

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        trackArea = nil;
        [self updateTrackingAreas];
    }
    return self;
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

- (void)updateTrackingAreas
{
    if (trackArea != nil)
    {
        [self removeTrackingArea:trackArea];
        [trackArea release];
    }
    
    NSTrackingAreaOptions options = (NSTrackingMouseMoved |
                                     NSTrackingMouseEnteredAndExited |
                                     NSTrackingActiveAlways); //same as Windows;
    
    trackArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                             options:options
                                               owner:self
                                            userInfo:nil];
    [self addTrackingArea:trackArea];
    [super updateTrackingAreas];
}

- (void)mouseMoved:(NSEvent *)event
{
    //NSLog(@"mouse move");
    NSPoint pt = [event locationInWindow];
    NSRect wnd_rect = [g_main_wnd.contentView frame];
    NSLog(@"x = %f, y = %f", pt.x, wnd_rect.size.height - pt.y);
    [g_main_wnd.contentView setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event
{
    NSLog(@"mouse drag");
}

- (void)mouseDown:(NSEvent *)event
{
    NSLog(@"LButton Down");
}

- (void)mouseUp:(NSEvent *)event
{
    NSLog(@"LButton UP");
}

- (void)rightMouseDown:(NSEvent *)event
{
    NSLog(@"RButton Down");
}

- (void)rightMouseUp:(NSEvent *)event
{
    NSLog(@"RButton UP");
}

- (void)drawRect:(NSRect)dirtyRect
{
    NSLog(@"Drawing");
    CGContextRef cg_ref = [[NSGraphicsContext currentContext] CGContext];
    NSColor* col = [NSColor colorWithCalibratedRed:(CGFloat)0/255
                                             green:(CGFloat)200/255
                                              blue:(CGFloat)200/255
                                             alpha:(CGFloat)1.0];
    [col set];
    static CGFloat i = 0;
    i += 0.5f;
    CGContextStrokeRectWithWidth(cg_ref, CGRectMake(10 + i, 10 + i, 50, 50), 1.0f);
}

@end
