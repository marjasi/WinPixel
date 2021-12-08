#include <tchar.h>
#include <windows.h>
#include "file_IO.h"
#include "error_handling.h"
#include "user_interface.h"

HBITMAP GetBimapHandleOfDrawArea(int bitmapWidth, int bitmapHeight, COLORREF* rgbColorValues)
{
    return CreateBitmap(
    bitmapWidth, //Bitmap width in pixels.
    bitmapHeight, //Bitmap height in pixels.
    1, //The number of color planes used in the bitmap file.
    sizeof(rgbColorValues) * 8, //Number of bits used to represent different colors.
    rgbColorValues //Bitmap color data array.
    );
}

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;

    //Retrieve the bitmap color format, width, and height.
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
    {
        CreateErrorMessageBox(hwnd, TEXT("Couldn't retrieve bitmap object."));
    }

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
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits));
    }
    //There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel.
    else
    {
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));
    }

    //Initialize the fields in the BITMAPINFO structure.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

    if (cClrBits < 24)
    {
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits);
    }

    //If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    //Compute the number of bytes in the array of color indices and store the result in biSizeImage.
    //The width must be DWORD aligned unless the bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) / 8 * pbmi->bmiHeader.biHeight;

    //Set biClrImportant to 0, indicating that all of the device colors are important.
    pbmi->bmiHeader.biClrImportant = 0;
    return pbmi;
}

void CreateBMPFile(HWND hwnd, HBITMAP hBMP, HDC hDC, LPTSTR bmpFileName, PBITMAPINFO pbi)
{
    HANDLE hf; //File handle.
    BITMAPFILEHEADER hdr; //Bitmap file-header.
    PBITMAPINFOHEADER pbih; //Bitmap info-header.
    LPBYTE lpBits; //Memory pointer.
    DWORD dwTotal; //Total count of bytes.
    DWORD cb; //Incremental count of bytes.
    BYTE *hp; //Byte pointer.
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits)
    {
        CreateErrorMessageBox( hwnd, TEXT("Couldn't allocate memory for bmp file data."));
    }

    //Retrieve the color table (RGBQUAD array) and the bits (array of palette indices) from the DIB.
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS))
    {
        CreateErrorMessageBox(hwnd, TEXT("Couldn't retrieve color table and palette indices from the DIB."));
    }

    //Create the .BMP file.
    hf = CreateFile(bmpFileName,
                   GENERIC_READ | GENERIC_WRITE,
                   (DWORD) 0,
                    NULL,
                   CREATE_ALWAYS,
                   FILE_ATTRIBUTE_NORMAL,
                   (HANDLE) NULL);

    if (hf == INVALID_HANDLE_VALUE)
    {
        CreateErrorMessageBox(hwnd, TEXT("Invalid bmp file handle."));
    }

    hdr.bfType = 0x4d42; //0x42 = "B", 0x4d = "M".

    //Compute the size of the entire file.
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    //Compute the offset to the array of color indices.
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD);

    //Copy the BITMAPFILEHEADER into the .BMP file.
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL))
    {
        CreateErrorMessageBox(hwnd, TEXT("Couldn't copy BITMAPFILEHEADER to bmp file."));
    }

    //Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, ( NULL)))
    {
        CreateErrorMessageBox(hwnd, TEXT("Couldn't copy BITMAPFILEHEADER to bmp file."));
    }

    //Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
    {
        CreateErrorMessageBox(hwnd, TEXT("Couldn't copy the array of palette indices to bmp file."));
    }

    //Close the .BMP file.
    if (!CloseHandle(hf))
    {
        CreateErrorMessageBox(hwnd, TEXT("Couldn't close the bmp file handle."));
    }

    //Free memory.
    GlobalFree((HGLOBAL)lpBits);
}
