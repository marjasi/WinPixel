#include <shlobj.h>
#include <objbase.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <shtypes.h>
#include <windows.h>
#include "error_handling.h"
#include "file_IO.h"
#include "user_interface.h"

//Win pixel dll handle and function type definitions.
HINSTANCE H_WIN_PIXEL_LIB;
//ConstructBmpInfo function type.
typedef PBITMAPINFO(__cdecl *WIN_PIXEL_LIB_CONSTR_BMP_INFO)(BITMAP);
//GetBmpRgb function type.
typedef void(__cdecl *WIN_PIXEL_LIB_GET_BMP_RGB)(HBITMAP, COLORREF*, int);

bool FileIoInit()
{
    H_WIN_PIXEL_LIB = LoadLibrary(L"dll\\winPixel.dll");
    return H_WIN_PIXEL_LIB != NULL;
}

//Loads the specified function from the win pixel dll library.
//IMPORTANT: the return value must be cast to a defined function type.
FARPROC LoadWinPixelDllFunction(LPCSTR functionName)
{
    return GetProcAddress(H_WIN_PIXEL_LIB, functionName);
}

bool FileIoFree()
{
    return FreeLibrary(H_WIN_PIXEL_LIB);
}

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
    WIN_PIXEL_LIB_CONSTR_BMP_INFO BmpInfoFunction;
    BITMAP bmp;
    PBITMAPINFO pbmi = NULL;

    //Retrieve the bitmap color format, width, and height.
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
    {
        CreateErrorMessageBox(hwnd, L"Couldn't retrieve bitmap object.");
        return NULL;
    }

    //Load bmp info struct creation function from the win pixel dll library.
    BmpInfoFunction = (WIN_PIXEL_LIB_CONSTR_BMP_INFO)LoadWinPixelDllFunction("ConstructBmpInfo");
    if (BmpInfoFunction != NULL)
    {
        pbmi = (BmpInfoFunction)(bmp);
    }
    
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
    WIN_PIXEL_LIB_GET_BMP_RGB BmpRgbFunction;
    
    //Load bmp pixel color data function from the win pixel dll library.
    BmpRgbFunction = (WIN_PIXEL_LIB_GET_BMP_RGB)LoadWinPixelDllFunction("GetBmpRgb");
    if (BmpRgbFunction != NULL)
    {
        (BmpRgbFunction)(hBMP, rgbColorValues, pixelAreaSize);
    }
}
