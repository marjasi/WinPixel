#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

#endif // FILE_IO_H_INCLUDED
