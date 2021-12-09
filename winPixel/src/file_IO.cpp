#include <shlobj.h>
#include <objbase.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <shtypes.h>
#include <windows.h>
#include "error_handling.h"
#include "file_IO.h"
#include "user_interface.h"

HBITMAP GetBimapHandleOfDrawArea(int bitmapWidth, int bitmapHeight, COLORREF* rgbColorValues)
{
    return CreateBitmap(
    bitmapWidth, //Bitmap width in pixels.
    bitmapHeight, //Bitmap height in pixels.
    1, //The number of color planes used in the bitmap file.
    sizeof(COLORREF) * 8, //Number of bits used to represent a color in a single pixel.
    rgbColorValues //Bitmap color data array.
    );
}

HBITMAP GetLoadedBitmapFileHandle(LPCWSTR bmpFileName)
{
    return (HBITMAP) LoadImageW(
    NULL, //Handle to a module that contains the bmp file.
    bmpFileName, //Path to bmp file.
    IMAGE_BITMAP, //Image type.
    0, //Width of the image to be loaded. If set to zero, loads the whole image.
    0, //Height of the image to be loaded. If set to zero, loads the whole image.
    LR_LOADFROMFILE //Additional flags.
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
        CreateErrorMessageBox(hwnd, L"Couldn't retrieve bitmap object.");
        return NULL;
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
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
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

LPWSTR ShowFileSaveWindowAndGetBmpFileLocation()
{
    //This File Save Dialog only allows to save bmp files.
    COMDLG_FILTERSPEC fdSaveTypes[] = {L"Bitmap (*.bmp)", L"*.bmp"};
    PWSTR bmpFilePath = NULL;
    // CoCreate the File Save Dialog object.
    IFileDialog *pfd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        //Set the options on the dialog.
        DWORD dwFlags;

        //Before setting, always get the options first in order not to override existing options.
        hr = pfd->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            //In this case, get shell items only for file system items.
            hr = pfd->SetOptions(dwFlags | FOS_OVERWRITEPROMPT | FOS_NOREADONLYRETURN
                                  | FOS_PATHMUSTEXIST | FOS_NOCHANGEDIR | FOS_STRICTFILETYPES);
            if (SUCCEEDED(hr))
            {
                //Set the file types to display only.
                //Notice that this is a 1-based array.
                hr = pfd->SetFileTypes(ARRAYSIZE(fdSaveTypes), fdSaveTypes);
                if (SUCCEEDED(hr))
                {
                    //Set the selected file type index to bmp files.
                    hr = pfd->SetFileTypeIndex(INDEX_BITMAP);
                    if (SUCCEEDED(hr))
                    {
                        //Set the default extension to be ".bmp" file.
                        hr = pfd->SetDefaultExtension(L"bmp");
                        if (SUCCEEDED(hr))
                        {
                            //Show the dialog.
                            hr = pfd->Show(NULL);
                            if (SUCCEEDED(hr))
                            {
                                //Obtain the result once the user clicks the 'Save' button.
                                //The result is an IShellItem object.
                                IShellItem *psiResult;
                                hr = pfd->GetResult(&psiResult);
                                if (SUCCEEDED(hr))
                                {
                                    //Get the bmp file location path string.
                                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &bmpFilePath);
                                    psiResult->Release();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    pfd->Release();
    return bmpFilePath;
}

LPWSTR ShowFileOpenWindowAndGetBmpFileLocation()
{
    //This File Load Dialog only allows to load bmp files.
    COMDLG_FILTERSPEC fdLoadTypes[] = { L"Bitmap (*.bmp)", L"*.bmp" };
    PWSTR bmpFilePath = NULL;
    // CoCreate the File Load Dialog object.
    IFileDialog* pfd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        //Set the options on the dialog.
        DWORD dwFlags;

        //Before setting, always get the options first in order not to override existing options.
        hr = pfd->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            //In this case, get shell items only for file system items.
            hr = pfd->SetOptions(dwFlags | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR);
            if (SUCCEEDED(hr))
            {
                //Set the file types to display only.
                //Notice that this is a 1-based array.
                hr = pfd->SetFileTypes(ARRAYSIZE(fdLoadTypes), fdLoadTypes);
                if (SUCCEEDED(hr))
                {
                    //Set the selected file type index to bmp files.
                    hr = pfd->SetFileTypeIndex(INDEX_BITMAP);
                    if (SUCCEEDED(hr))
                    {
                        //Set the default extension to be ".bmp" file.
                        hr = pfd->SetDefaultExtension(L"bmp");
                        if (SUCCEEDED(hr))
                        {
                            //Show the dialog.
                            hr = pfd->Show(NULL);
                            if (SUCCEEDED(hr))
                            {
                                //Obtain the result once the user clicks the 'Open' button.
                                //The result is an IShellItem object.
                                IShellItem* psiResult;
                                hr = pfd->GetResult(&psiResult);
                                if (SUCCEEDED(hr))
                                {
                                    //Get the bmp file location path string.
                                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &bmpFilePath);
                                    psiResult->Release();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    pfd->Release();
    return bmpFilePath;
}

void CreateBitmapFile(HWND hwnd, HBITMAP hBMP, HDC hDC, LPWSTR bmpFileName, PBITMAPINFO pbi)
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
        CreateErrorMessageBox( hwnd, L"Couldn't allocate memory for bmp file data.");
        return;
    }

    //Retrieve the color table (RGBQUAD array) and the bits (array of palette indices) from the DIB.
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS))
    {
        CreateErrorMessageBox(hwnd, L"Couldn't retrieve color table and palette indices from the DIB.");
        GlobalFree((HGLOBAL)lpBits);
        return;
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
        CreateErrorMessageBox(hwnd, L"Invalid bmp file handle.");
        GlobalFree((HGLOBAL)lpBits);
        return;
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
        CreateErrorMessageBox(hwnd, L"Couldn't copy BITMAPFILEHEADER to bmp file.");
        GlobalFree((HGLOBAL)lpBits);
        return;
    }

    //Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, ( NULL)))
    {
        CreateErrorMessageBox(hwnd, L"Couldn't copy BITMAPFILEHEADER to bmp file.");
        GlobalFree((HGLOBAL)lpBits);
        return;
    }

    //Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
    {
        CreateErrorMessageBox(hwnd, L"Couldn't copy the array of palette indices to bmp file.");
        GlobalFree((HGLOBAL)lpBits);
        return;
    }

    //Close the .BMP file.
    if (!CloseHandle(hf))
    {
        CreateErrorMessageBox(hwnd, L"Couldn't close the bmp file handle.");
    }

    //Free memory.
    GlobalFree((HGLOBAL)lpBits);
}

void GetBitmapPixelColorData(HBITMAP hBMP, COLORREF* rgbColorValues, int pixelAreaSize)
{
    GetBitmapBits(hBMP, sizeof(COLORREF) * pixelAreaSize, rgbColorValues);
}
