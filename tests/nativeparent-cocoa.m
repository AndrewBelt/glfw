#import <Cocoa/Cocoa.h>


void* createNativeWindow()
{
    NSRect contentRect = NSMakeRect(0, 0, 300, 200);
    NSUInteger styleMask = NSWindowStyleMaskTitled |
        NSWindowStyleMaskClosable |
        NSWindowStyleMaskMiniaturizable |
        NSWindowStyleMaskResizable;

    NSWindow* window = [[NSWindow alloc]
        initWithContentRect:contentRect
        styleMask:styleMask
        backing:NSBackingStoreBuffered
        defer:NO];

    return (void*) window;
}

void destroyNativeWindow(void* handle)
{
    NSWindow* window = (NSWindow*) handle;
    [window close];
    [window release];
}
