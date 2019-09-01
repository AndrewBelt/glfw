#include <X11/Xlib.h>

static Display* display = NULL;

void* createNativeWindow()
{
    display = XOpenDisplay(NULL);
    if (!display)
        return NULL;

    int screen = DefaultScreen(display);
    Window handle = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 300, 200, 20, BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, handle, ExposureMask | KeyPressMask);
    XMapWindow(display, handle);

    // Consume events until the window is exposed (shown).
    while (1) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == Expose)
            break;
    }

    return (void*) handle;
}

void destroyNativeWindow(void* handle)
{
    XDestroyWindow(display, (Window) handle);
    XCloseDisplay(display);
}
