#ifndef WIN_PIXEL_LIBRARY_H_INCLUDED
#define WIN_PIXEL_LIBRARY_H_INCLUDED

#include <windows.h>

#ifdef WINPIXELLIBRARYLIBRARY_EXPORTS
#define WIN_PIXEL_LIBRARY __declspec(dllexport)
#else
#define WIN_PIXEL_LIBRARY __declspec(dllimport)
#endif

//Constructs a PBITMAPINFO struct based on the provided bitmap header.
extern "C" WIN_PIXEL_LIBRARY PBITMAPINFO ConstructBmpInfo(BITMAP bmp);
//Returns the specified size area pixel color values of a loaded bmp file in the passed parameter rgbColorValues.
extern "C" WIN_PIXEL_LIBRARY void GetBmpRgb(HBITMAP hBMP, COLORREF * rgb, int pixelCount);

#endif // WIN_PIXEL_LIBRARY_H_INCLUDED