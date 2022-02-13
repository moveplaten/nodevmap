/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#import <Cocoa/Cocoa.h>

#include "draw/nvp_draw.h"
#include "base/nvp_base.h"

extern NSWindow* g_main_wnd;
extern CGContextRef g_cg_ref;
extern CTFontRef g_ct_font_ref;

@interface WndDelegate : NSObject <NSWindowDelegate, NSApplicationDelegate>

@end

@interface MainView : NSView

@end
