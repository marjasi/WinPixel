#include "win_pixel_library.h"

PBITMAPINFO ConstructBmpInfo(BITMAP bmp)
{
    PBITMAPINFO pbmi;
    WORD cClrBits;

    //Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

    if (cClrBits == 1)
    {
        cClrBits = 1;
    }
    else if (cClrBits <= 4)
    {
        cClrBits = 4;
    }
    else if (cClrBits <= 8)
    {
        cClrBits = 8;
    }
    else if (cClrBits <= 16)
    {
        cClrBits = 16;
    }
    else if (cClrBits <= 24)
    {
        cClrBits = 24;
    }
    else
    {
        cClrBits = 32;
    }

    //Allocate memory for the BITMAPINFO structure (this structure contains a BITMAPINFOHEADER structure and an array of RGBQUAD
    // data structures).
    if (cClrBits < 24)
    {
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
    }
    //There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel.
    else
    {
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));
    }

    //Initialize the fields in the BITMAPINFO structure.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

    if (cClrBits < 24)
    {
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
    }

    //If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    //Compute the number of bytes in the array of color indices and store the result in biSizeImage.
    //The width must be DWORD aligned unless the bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 * pbmi->bmiHeader.biHeight;

    //Set biClrImportant to 0, indicating that all of the device colors are important.
    pbmi->bmiHeader.biClrImportant = 0;
    return pbmi;
}

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