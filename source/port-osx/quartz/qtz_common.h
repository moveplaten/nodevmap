#import <Cocoa/Cocoa.h>

#include "draw/draw.h"
#include "base/base.h"

extern NSWindow* g_main_wnd;
extern CGContextRef g_cg_ref;

@interface WndDelegate : NSObject <NSWindowDelegate, NSApplicationDelegate>

@end

@interface MainView : NSView

@end


class QtzNvpDrawPort : public NvpDrawPort
{
    virtual void beginDraw() override;
    
    virtual void fillRect(const BaseRect& rect, NvpColor colo) override;
    virtual void frameRect(const BaseRect& rect, NvpColor colo) override;
    
};
