#include <windows.h>


void* createNativeWindow()
{
    HWND handle = CreateWindowExW(0, L"PARENT", L"Parent window", WS_OVERLAPPEDWINDOW, 0, 0, 400, 600, NULL, NULL, GetModuleHandleW(NULL), NULL);
    ShowWindow(handle, SW_SHOWNORMAL);
    return (void*) handle;
}

void destroyNativeWindow(void* handle)
{
    DestroyWindow((HWND) handle);
}
