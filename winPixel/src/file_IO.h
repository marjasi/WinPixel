#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED

//File type indices
#define INDEX_BITMAP 1

//Returns a bitmap file handle for the current color data in the draw area.
HBITMAP GetBimapHandleOfDrawArea(int bitmapWidth, int bitmapHeight, COLORREF* rgbColorValues);
//Create a BITMAPINFO struct based on the handle to a bitmap.
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
//Shows a file save dialog and returns the selected new bmp file save location.
//IMPORTANT: requires the COM library to be initialized on the current process.
LPWSTR ShowFileSaveWindowAndGetBmpFileLocation();
//Create a bmp file based on its bit data.
void CreateBMPFile(HWND hwnd, HBITMAP hBMP, HDC hDC, LPWSTR bmpFileName, PBITMAPINFO pbi);

#endif // FILE_IO_H_INCLUDED