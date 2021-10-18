#import <Cocoa/Cocoa.h>

#include "draw/draw.h"
#include "base/base.h"

extern NSWindow* g_main_wnd;
extern CGContextRef g_cg_ref;
extern CTFontRef g_ct_font_ref;

@interface WndDelegate : NSObject <NSWindowDelegate, NSApplicationDelegate>

@end

@interface MainView : NSView

@end
