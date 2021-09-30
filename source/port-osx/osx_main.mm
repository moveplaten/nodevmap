#import <Cocoa/Cocoa.h>
#include "base/base.h"

@interface WndDelegate : NSObject <NSWindowDelegate, NSApplicationDelegate>
@end
@interface MainView : NSView
@end
extern NSWindow* g_main_wnd;

int main(int argc, const char * argv[])
{
    NvpLevel::iterator iter;
    BaseElement b(0, 0, 0, 0, iter);
    elemGen("test0", MsgNone, nullptr);
    ElemStorage<100, int> e;
    
    [NSApplication sharedApplication];
    
    //no dock icon, no menu bar;
    BOOL OK = [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];
    if (OK == NO)
    {
        NSLog(@"setActivationPolicy Failed");
        return 1;
    }
    
    //create Cocoa window;
    int x, y, width, height;
    x = y = 100;
    width = height = 600;
    NSRect windowRect = NSMakeRect(x, y, width, height);
    
    NSWindowStyleMask windowStyle = (NSWindowStyleMaskTitled |
                                     NSWindowStyleMaskClosable |
                                     NSWindowStyleMaskMiniaturizable |
                                     NSWindowStyleMaskResizable);
    
    g_main_wnd = [[NSWindow alloc] initWithContentRect:windowRect
                                             styleMask:windowStyle
                                               backing:NSBackingStoreBuffered
                                                 defer:NO];
    if (g_main_wnd == nil)
    {
        NSLog(@"Create Window Failed");
        return 1;
    }
    
    WndDelegate* wndDelegate = [[WndDelegate alloc] init];
    [g_main_wnd setDelegate:wndDelegate];
    [NSApp setDelegate:wndDelegate];
    MainView* mainView = [[MainView alloc] init];
    [g_main_wnd setContentView:mainView];
    [g_main_wnd makeFirstResponder:mainView];
    [g_main_wnd setTitle:@"nodevmap"];
    [g_main_wnd setRestorable:NO];
    [g_main_wnd setLevel:NSFloatingWindowLevel]; //always on top;
    [g_main_wnd setAlphaValue:0.9f]; //transparent;
    
    //show the window;
    [g_main_wnd orderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];
    [g_main_wnd makeKeyAndOrderFront:NSApp];
    
    [wndDelegate release];
    [mainView release];
    
    [NSApp run]; //run message loop;
    
    return 0;
}
