#include "win_pixel_library.h"

void GetBmpRgb(HBITMAP hBMP, COLORREF* rgb, int pixelCount)
{
    GetBitmapBits(hBMP, sizeof(COLORREF) * pixelCount, rgb);
}

extern "C" BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        //Attach to process.
        //Return FALSE to fail DLL load.
        break;

    case DLL_PROCESS_DETACH:
        //Detach from process.
        break;

    case DLL_THREAD_ATTACH:
        //Attach to thread.
        break;

    case DLL_THREAD_DETACH:
        //Detach from thread.
        break;
    }
    return TRUE; //Succesful.
}