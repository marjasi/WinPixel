#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED

//File type indices
#define INDEX_BITMAP 1

//Initializes the dll library used in this file.
//Returns true if the initialization was successful, false otherwise.
//IMPORTANT: this must be called at the start of the program before using any of the below functions.
bool FileIoInit();
//Frees the dll library used in this file.
//Returns true if the dll library was freed successfully, false otherwise.
//IMPORTANT: this must be called if FileIoInit() was called before terminating the program.
bool FileIoFree();
//Returns a bitmap file handle for the current color data in the draw area.
HBITMAP GetBimapHandleOfDrawArea(int bitmapWidth, int bitmapHeight, COLORREF* rgbColorValues);
//Loads a bmp file and returns its handle.
HBITMAP GetLoadedBitmapFileHandle(LPCWSTR bmpFileName);
//Create a BITMAPINFO struct based on the handle to a bitmap.
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
//Shows a file save dialog and returns the selected new bmp file save location.
//IMPORTANT: requires the COM library to be initialized on the current process.
LPWSTR ShowFileSaveWindowAndGetBmpFileLocation();
//Shows a file open dialog and returns the selected existing bmp file location.
//IMPORTANT: requires the COM library to be initialized on the current process.
LPWSTR ShowFileOpenWindowAndGetBmpFileLocation();
//Create a bmp file based on its bit data.
void CreateBitmapFile(HWND hwnd, HBITMAP hBMP, HDC hDC, LPWSTR bmpFileName, PBITMAPINFO pbi);
//Returns the specified size area pixel color values of a loaded bmp file in the passed parameter rgbColorValues.
void GetBitmapPixelColorData(HBITMAP hBMP, COLORREF* rgbColorValues, int pixelAreaSize);

#endif // FILE_IO_H_INCLUDED
