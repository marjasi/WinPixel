#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED

//Create a BITMAPINFO struct based on the handle to a bitmap.
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
//Create a bmp file based on its bit data.
void CreateBMPFile(HWND hwnd, HBITMAP hBMP, HDC hDC, LPTSTR bmpFileName, PBITMAPINFO pbi);

#endif // FILE_IO_H_INCLUDED
