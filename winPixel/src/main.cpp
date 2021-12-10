#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <iostream>
#include <windows.h>
#include "error_handling.h"
#include "file_IO.h"
#include "user_interface.h"

//Custom functions.
//Checks whether a given value is in the range with the inclusive low and high values.
bool InRange(int value, int lowValue, int highValue);
//Returns true if the passed handle is a handle of the current color square, otherwise returns false.
bool HandleIsCurrentColorSquareHandle(HWND squareHandle, HWND currentColorSquareHandle);
//Returns true if the passed handle is a handle of a color palette oval, otherwise returns false.
bool HandleIsColorPaletteOvalHandle(HWND ovalHandle, HWND* allOvalHandles, int colorPaletteOvalNum);
//Returns true if the passed handle is a handle of a draw area square, otherwise returns false.
bool HandleIsDrawAreaSquareHandle(HWND squareHandle, HWND* allSquareHandles, int drawAreaSquareNum);
//Creates a draw area square that is an owner drawn button.
HWND CreateDrawAreaSquare(int x, int y, int width, int height, int buttonId, HWND hwnd);
//Loads the pixel color data from a file into the draw area and redraws the whole draw area.
void LoadPixelColorDataIntoDrawArea(COLORREF* drawAreaRgb, COLORREF* loadedFileRgb);
//Creates the current color square.
void CreateCurrentColorSquare(HWND hwnd);
//Creates all color palette ovals.
void CreateColorPalette(HWND hwnd);
//Creates all draw area squares.
void CreateDrawArea(HWND hwnd);
//Draws the current color square
void DrawCurrentColorSquare(LPDRAWITEMSTRUCT lpDIS, int squareColorRGB, HGDIOBJ drawingBrush, HGDIOBJ drawingPen, int squareBorderColor);
//Draws a certain color palette oval.
void DrawColorPaletteOval(LPDRAWITEMSTRUCT lpDIS, int ovalColorRGB, HGDIOBJ drawingBrush, HGDIOBJ drawingPen, int ovalBorderColor);
//Draws a certain draw area square.
void DrawDrawAreaSquare(LPDRAWITEMSTRUCT lpDIS, int squareColorRGB, HGDIOBJ drawingBrush, HGDIOBJ drawingPen, bool drawBorder);
//Redraws a certain draw area square.
void RedrawDrawAreaSquare(HWND squareHandle);
//Redraws all draw area squares.
void RedrawDrawArea();
//Takes the rgb values of the draw area rgbValues and stores them in bgr format in bgrValues.
void FillBgrColorsOfDrawArea(COLORREF* rgbValues, COLORREF* bgrValues, int drawAreaSquareNum);
//Takes the bgr values from a loaded bitmap file loadedFilebgrValues and stores them in rgb format in loadedFilergbValues.
void FillRgbColorsOfLoadedFile(COLORREF* loadedFileBgrValues, COLORREF* loadedFileRgbValues, int drawAreaSquareNum);
//Returns the color palette oval sequence number based on its handle.
//Returns -1 if there is no such color palette oval with the specified handle value.
int GetColorPaletteOvalSeqNumByHandle(HWND ovalHandle, HWND* allOvalHandles, int colorPaletteOvalNum);
//Returns the draw area square sequence number based on its handle.
//Returns -1 if there is no such draw area square with the specified handle value.
int GetDrawAreaSquareSeqNumByHandle(HWND squareHandle, HWND* allSquareHandles, int drawAreaSquareNum);

//Declare Windows procedure.
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

//Make the class name into a global variable.
TCHAR szClassName[] = TEXT("WinPixel");

//Constant global variables.
const int MAIN_WINDOW_WIDTH = 900;
const int MAIN_WINDOW_HEIGHT = 800;
const int DRAW_AREA_SQUARE_NUM = DRAW_AREA_WIDTH * DRAW_AREA_HEIGHT;
const int COLOR_PALETTE_OVAL_NUM = COLOR_PALETTE_WIDTH * COLOR_PALETTE_HEIGHT;
//Low and high bound values are inclusive.
const int COLOR_PALETTE_LOW_VALUE = ID_COLOR_PALETTE_OVAL_1;
const int COLOR_PALETTE_HIGH_VALUE = COLOR_PALETTE_LOW_VALUE + COLOR_PALETTE_OVAL_NUM -1;
const int DRAW_AREA_LOW_VALUE = ID_DRAW_AREA_SQUARE_1;
const int DRAW_AREA_HIGH_VALUE = DRAW_AREA_LOW_VALUE + DRAW_AREA_SQUARE_NUM - 1;

//Global variables.
int DEFAULT_DRAW_AREA_SQUARE_COLOR = RGB(255, 255, 255);
int DEFAULT_DRAW_AREA_SQUARE_BORDER_COLOR = RGB(0, 0, 0);
int DEFAULT_CURRENT_COLOR_SQUARE_BORDER_COLOR = RGB(0, 0, 0);
int DEFAULT_COLOR_PALETTE_OVAL_BORDER_COLOR = RGB(0, 0, 0);
int CURRENT_COLOR_SQUARE_X = DRAW_AREA_WIDTH * DRAW_AREA_SQUARE_WIDTH + 40;
int CURRENT_COLOR_SQUARE_Y = DRAW_AREA_TOP_LEFT_Y + 5;
int COLOR_PALETTE_TOP_LEFT_X = CURRENT_COLOR_SQUARE_X - 5;
int COLOR_PALETTE_TOP_LEFT_Y = CURRENT_COLOR_SQUARE_Y + CURRENT_COLOR_HEIGHT + COLOR_PALETTE_OVAL_SPACING_VERTICAL;
bool DRAW_DRAW_AREA_SQUARE_BORDER = true;
//Current color square handle.
HWND CURRENT_COLOR_SQUARE_HANDLE;
//All the color palette oval handles.
HWND COLOR_PALETTE_OVAL_HANDLES[COLOR_PALETTE_OVAL_NUM];
//All the draw area square handles.
HWND DRAW_AREA_SQUARE_HANDLES[DRAW_AREA_SQUARE_NUM];
//The current color of all color palette ovals.
COLORREF COLOR_PALETTE_OVAL_RGB[COLOR_PALETTE_OVAL_NUM];
//The current color of all draw area squares in rgb format.
COLORREF DRAW_AREA_SQUARE_RGB[DRAW_AREA_SQUARE_NUM];
//The current color of all draw area squares in bgr format.
//Used to save the draw area into a bmp file, because by defauts the functions used for saving
// a bmp file operate with bgr.
COLORREF DRAW_AREA_SQUARE_BGR[DRAW_AREA_SQUARE_NUM];
//A loaded bmp file rgb color values.
COLORREF LOADED_BMP_RGB[DRAW_AREA_SQUARE_NUM];
//A loaded bmp file bgr color values.
COLORREF LOADED_BMP_BGR[DRAW_AREA_SQUARE_NUM];
//The current draw color.
//It is the color black by default.
COLORREF CURRENT_DRAW_COLOR = RGB(0, 0, 0);

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    //Initialize the COM library.
    //If the initialization fails, we quit the program.
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (!SUCCEEDED(hr))
    {
        return -1;
    }

    //Initialize the dll library for the file_IO functions.
    //If the initialization fails, we quit the program.
    if (!FileIoInit())
    {
        //Uninitialize the COM library.
        CoUninitialize();
        return -2;
    }

    HWND hwnd; //This is the handle for our window.
    MSG messages; //Here messages to the application are saved.
    WNDCLASSEX wincl; //Data structure for the windowclass.

    //The Window structure.
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure; //This function is called by windows.
    wincl.style = CS_DBLCLKS; //Catch double-clicks.
    wincl.cbSize = sizeof (WNDCLASSEX);

    //Use default icon and mouse-pointer.
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL; //No menu.
    wincl.cbClsExtra = 0; //No extra bytes after the window class.
    wincl.cbWndExtra = 0; //Structure or the window instance.

    //Use Windows's default colour as the background of the window.
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(242, 242, 242));

    //Register the window class, and if it fails quit the program.
    if (!RegisterClassEx (&wincl))
    {
        //Uninitialize the COM library.
        CoUninitialize();
        //Uninitialize the dll library used in file_IO.
        FileIoFree();
        return -3;
    }

    //The class is registered, let's create the program.
    hwnd = CreateWindowEx (
    0, //Extended possibilites for variation.
    szClassName, //Classname.
    TEXT("WinPixel"), //Title Text.
    WS_OVERLAPPEDWINDOW, //Default window.
    CW_USEDEFAULT, //Windows decides the position.
    CW_USEDEFAULT, //where the window ends up on the screen.
    MAIN_WINDOW_WIDTH, //The program's width in pixels.
    MAIN_WINDOW_HEIGHT, //The program's height in pixels.
    HWND_DESKTOP, //The window is a child-window to desktop.
    LoadMenu(hThisInstance, MAKEINTRESOURCE(ID_TOP_MENU)), //Menu from resource file.
    hThisInstance, //Program Instance handler.
    NULL //No Window Creation data.
    );

    //Make the window visible on the screen.
    ShowWindow (hwnd, nCmdShow);

    //Run the message loop. It will run until GetMessage() returns 0.
    while (GetMessage (&messages, NULL, 0, 0))
    {
        //Translate virtual-key messages into character messages.
        TranslateMessage(&messages);
        //Send message to WindowProcedure.
        DispatchMessage(&messages);
    }

    //Uninitialize the COM library.
    CoUninitialize();
    //Uninitialize the dll library used in file_IO.
    FileIoFree();
    //The program return-value is 0 - The value that PostQuitMessage() gave.
    return messages.wParam;
}

//Checks whether a given value is in the range with the inclusive low and high values.
bool InRange(int value, int lowValue, int highValue)
{
    return (lowValue <= value && value <= highValue);
}

//Returns true if the passed handle is a handle of the current color square, otherwise returns false.
bool HandleIsCurrentColorSquareHandle(HWND squareHandle, HWND currentColorSquareHandle)
{
    return squareHandle == currentColorSquareHandle;
}

//Returns true if the passed handle is a handle of a color palette oval, otherwise returns false.
bool HandleIsColorPaletteOvalHandle(HWND ovalHandle, HWND* allOvalHandles, int colorPaletteOvalNum)
{
    int i;
    for (i = 0; i < colorPaletteOvalNum; i++)
    {
        if (allOvalHandles[i] == ovalHandle)
        {
            return true;
        }
    }

    return false;
}

//Returns true if the passed handle is a handle of a draw area square, otherwise returns false.
bool HandleIsDrawAreaSquareHandle(HWND squareHandle, HWND* allSquareHandles, int drawAreaSquareNum)
{
    int i;
    for (i = 0; i < drawAreaSquareNum; i++)
    {
        if (allSquareHandles[i] == squareHandle)
        {
            return true;
        }
    }

    return false;
}

//Creates a draw area square that is an owner drawn button.
HWND CreateDrawAreaSquare(int x, int y, int width, int height, int buttonId, HWND hwnd)
{
    return CreateWindow(
    TEXT("BUTTON"), //The draw area square is a button.
    L"", //No button text.
    WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, //Styles.
    x, //x axis position.
    y, //y axis position.
    width, //Width.
    height, //Height.
    hwnd, //Parent window of the child window.
    (HMENU) buttonId, //The ID of the button.
    (HINSTANCE) GetWindowLong(hwnd, GWLP_HINSTANCE), //Program instance.
    NULL
    );
}

//Loads the pixel color data from a file into the draw area and redraws the whole draw area.
void LoadPixelColorDataIntoDrawArea(COLORREF* drawAreaRgb, COLORREF* loadedFileRgb)
{
    int i;
    for (i = 0; i < DRAW_AREA_SQUARE_NUM; i++)
    {
        drawAreaRgb[i] = loadedFileRgb[i];
    }
    RedrawDrawArea();
}

//Creates the current color square.
void CreateCurrentColorSquare(HWND hwnd)
{
    CURRENT_COLOR_SQUARE_HANDLE = CreateDrawAreaSquare(CURRENT_COLOR_SQUARE_X,
        CURRENT_COLOR_SQUARE_Y, CURRENT_COLOR_WIDTH, CURRENT_COLOR_HEIGHT, ID_CURRENT_COLOR, hwnd);
}

//Creates all color palette ovals.
void CreateColorPalette(HWND hwnd)
{
    int i;
    int currentRowNum = 1;
    int currentColumnNum = 0;
    //The first oval is drawn in the top left corner.
    int ovalXPos = COLOR_PALETTE_TOP_LEFT_X;
    int ovalYPos = COLOR_PALETTE_TOP_LEFT_Y;

    for (i = 0; i < COLOR_PALETTE_OVAL_NUM; i++)
    {
        //Row still not filled.
        if (currentColumnNum < COLOR_PALETTE_WIDTH)
        {
            ovalXPos = COLOR_PALETTE_TOP_LEFT_X + COLOR_PALETTE_OVAL_WIDTH * currentColumnNum;
            //Add spacing for ovals.
            if (currentColumnNum != 0)
            {
                ovalXPos = ovalXPos + COLOR_PALETTE_OVAL_SPACING_HORIZONTAL;
            }
            currentColumnNum++;
        }
        //New row start x position.
        else
        {
            ovalXPos = COLOR_PALETTE_TOP_LEFT_X;
            ovalYPos = COLOR_PALETTE_TOP_LEFT_Y + COLOR_PALETTE_OVAL_HEIGHT * currentRowNum + COLOR_PALETTE_OVAL_SPACING_VERTICAL * currentRowNum;
            currentColumnNum = 1;
            currentRowNum++;
        }

        //Create a color palette oval and store its handle for later use.
        //There is no difference if we create a draw area square, because the following function doesn't affect the way a created button is drawn.
        COLOR_PALETTE_OVAL_HANDLES[i] = CreateDrawAreaSquare(ovalXPos, ovalYPos, COLOR_PALETTE_OVAL_WIDTH, COLOR_PALETTE_OVAL_HEIGHT, ID_COLOR_PALETTE_OVAL_1 + i, hwnd);
    }

    //All the color palette oval colors are set manually, because each oval has a different color - no way to really use a for loop.
    COLOR_PALETTE_OVAL_RGB[0] = CURRENT_DRAW_COLOR; //The current draw color - by default it is black.
    COLOR_PALETTE_OVAL_RGB[1] = RGB(192, 192, 192); //Light grey.
    COLOR_PALETTE_OVAL_RGB[2] = RGB(255, 255, 255); //White.
    COLOR_PALETTE_OVAL_RGB[3] = RGB(153, 0, 0); //Dark red.
    COLOR_PALETTE_OVAL_RGB[4] = RGB(255, 0, 0); //Red.
    COLOR_PALETTE_OVAL_RGB[5] = RGB(255, 204, 204); //Light red.
    COLOR_PALETTE_OVAL_RGB[6] = RGB(189, 0, 183); //Darker purple.
    COLOR_PALETTE_OVAL_RGB[7] = RGB(126, 1, 250); //Blue purple.
    COLOR_PALETTE_OVAL_RGB[8] = RGB(0, 171, 171); //Turqoise
    COLOR_PALETTE_OVAL_RGB[9] = RGB(0, 0, 255); //Blue.
    COLOR_PALETTE_OVAL_RGB[10] = RGB(0, 222, 255); //Teal.
    COLOR_PALETTE_OVAL_RGB[11] = RGB(0, 255, 222); //Green Teal.
    COLOR_PALETTE_OVAL_RGB[12] = RGB(0, 255, 43); //Light green.
    COLOR_PALETTE_OVAL_RGB[13] = RGB(45, 159, 45); //Dark green.
    COLOR_PALETTE_OVAL_RGB[14] = RGB(145, 255, 0); //Yellow green.
    COLOR_PALETTE_OVAL_RGB[15] = RGB(255, 255, 0); //Yellow.
    COLOR_PALETTE_OVAL_RGB[16] = RGB(255, 128, 0); //Orange
    COLOR_PALETTE_OVAL_RGB[17] = RGB(255, 229, 204); //Light orange.
}

//Creates all draw area squares.
void CreateDrawArea(HWND hwnd)
{
    int i;
    int currentRowNum = 1;
    int currentColumnNum = 0;
    //The first square is drawn in the top left corner.
    int squareXPos = DRAW_AREA_TOP_LEFT_X;
    int squareYPos = DRAW_AREA_TOP_LEFT_Y;
    for (i = 0; i < DRAW_AREA_SQUARE_NUM; i++)
    {
        //Row still not filled.
        if (currentColumnNum < DRAW_AREA_WIDTH)
        {
            squareXPos = DRAW_AREA_TOP_LEFT_X + DRAW_AREA_SQUARE_WIDTH * currentColumnNum;
            currentColumnNum++;
        }
        //New row start x position.
        else
        {
            squareXPos = DRAW_AREA_TOP_LEFT_X;
            squareYPos = DRAW_AREA_TOP_LEFT_Y + DRAW_AREA_SQUARE_HEIGHT * currentRowNum;
            currentColumnNum = 1;
            currentRowNum++;
        }
        //Create a draw area square and store its handle for later use.
        DRAW_AREA_SQUARE_HANDLES[i] = CreateDrawAreaSquare(squareXPos, squareYPos, DRAW_AREA_SQUARE_WIDTH, DRAW_AREA_SQUARE_HEIGHT, ID_DRAW_AREA_SQUARE_1 + i, hwnd);
        //Set the default square color for drawing.
        DRAW_AREA_SQUARE_RGB[i] = DEFAULT_DRAW_AREA_SQUARE_COLOR;
    }
}

//Draws the current color square
void DrawCurrentColorSquare(LPDRAWITEMSTRUCT lpDIS, int squareColorRGB, HGDIOBJ drawingBrush, HGDIOBJ drawingPen, int squareBorderColor)
{
    SetDCPenColor(lpDIS->hDC, squareBorderColor);
    SetDCBrushColor(lpDIS->hDC, squareColorRGB);
    SelectObject(lpDIS->hDC, drawingBrush);
    SelectObject(lpDIS->hDC, drawingPen);
    Rectangle(lpDIS->hDC, lpDIS->rcItem.left, lpDIS->rcItem.top,
        lpDIS->rcItem.right, lpDIS->rcItem.bottom);
}

//Draws a certain color palette oval.
void DrawColorPaletteOval(LPDRAWITEMSTRUCT lpDIS, int ovalColorRGB, HGDIOBJ drawingBrush, HGDIOBJ drawingPen, int ovalBorderColor)
{
    SetDCPenColor(lpDIS->hDC, ovalBorderColor);
    SetDCBrushColor(lpDIS->hDC, ovalColorRGB);
    SelectObject(lpDIS->hDC, drawingBrush);
    SelectObject(lpDIS->hDC, drawingPen);
    RoundRect(lpDIS->hDC, lpDIS->rcItem.left, lpDIS->rcItem.top,
        lpDIS->rcItem.right, lpDIS->rcItem.bottom, COLOR_PALETTE_OVAL_WIDTH, COLOR_PALETTE_OVAL_HEIGHT);;
}

//Draws a certain draw area square.
void DrawDrawAreaSquare(LPDRAWITEMSTRUCT lpDIS, int squareColorRGB, HGDIOBJ drawingBrush, HGDIOBJ drawingPen, bool drawBorder)
{
    //If we don't want a visible border, we just set the border's color to the background color of the square.
    int squareBorderColor = drawBorder ? DEFAULT_DRAW_AREA_SQUARE_BORDER_COLOR : squareColorRGB;

    SetDCPenColor(lpDIS -> hDC, squareBorderColor);
    SetDCBrushColor(lpDIS -> hDC, squareColorRGB);
    SelectObject(lpDIS -> hDC, drawingBrush);
    SelectObject(lpDIS -> hDC, drawingPen);
    Rectangle(lpDIS -> hDC, lpDIS -> rcItem.left, lpDIS -> rcItem.top,
        lpDIS -> rcItem.right, lpDIS -> rcItem.bottom);
}

//Redraws a certain draw area square.
void RedrawDrawAreaSquare(HWND squareHandle)
{
    //Sends a WM_DRAWITEM message for the specified window, which is a draw area square in this case.
    InvalidateRect(squareHandle, NULL, false);
}

//Redraws all draw area squares.
void RedrawDrawArea()
{
    int i;
    for (i = 0; i < DRAW_AREA_SQUARE_NUM; i++)
    {
        RedrawDrawAreaSquare(DRAW_AREA_SQUARE_HANDLES[i]);
    }
}

//Takes the rgb values of the draw area rgbValues and stores them in bgr format in bgrValues.
void FillBgrColorsOfDrawArea(COLORREF* rgbValues, COLORREF* bgrValues, int drawAreaSquareNum)
{
    int i;
    for (i = 0; i < drawAreaSquareNum; i++)
    {
        bgrValues[i] = RGB(GetBValue(rgbValues[i]), GetGValue(rgbValues[i]), GetRValue(rgbValues[i]));
    }
}

//Takes the bgr values from a loaded bitmap file loadedFilebgrValues and stores them in rgb format in loadedFilergbValues.
void FillRgbColorsOfLoadedFile(COLORREF* loadedFileBgrValues, COLORREF* loadedFileRgbValues, int drawAreaSquareNum)
{
    int i;
    for (i = 0; i < drawAreaSquareNum; i++)
    {
        //GetBValue is supposed to be used with rgb balues and in that case it does return the blue color channel value.
        //But if you use it with bgr it will return the red channel value, because GetBValue actually returns the last channel value
        // of a given color format (rgb, bgr or other).
        //GetRValue behaves the same except it returns the first channel value.
        loadedFileRgbValues[i] = RGB(GetBValue(loadedFileBgrValues[i]), GetGValue(loadedFileBgrValues[i]), GetRValue(loadedFileBgrValues[i]));
    }
}

//Returns the color palette oval sequence number based on its handle.
//Returns -1 if there is no such color palette oval with the specified handle value.
int GetColorPaletteOvalSeqNumByHandle(HWND ovalHandle, HWND* allOvalHandles, int colorPaletteOvalNum)
{
    int i;
    for (i = 0; i < colorPaletteOvalNum; i++)
    {
        if (allOvalHandles[i] == ovalHandle)
        {
            return i;
        }
    }

    return -1;
}

//Returns the draw area square sequence number based on its handle.
//Returns -1 if there is no such draw area square with the specified handle value.
int GetDrawAreaSquareSeqNumByHandle(HWND squareHandle, HWND* allSquareHandles, int drawAreaSquareNum)
{
    int i;
    for (i = 0; i < drawAreaSquareNum; i++)
    {
        if (allSquareHandles[i] == squareHandle)
        {
            return i;
        }
    }

    return -1;
}

//This function is called by the Windows function DispatchMessage().
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message) //Handle the messages.
    {
        case WM_CREATE:
            CreateCurrentColorSquare(hwnd);
            CreateColorPalette(hwnd);
            CreateDrawArea(hwnd);
            break;
        case WM_PAINT:
            PAINTSTRUCT ps;
            hdc = BeginPaint(hwnd, &ps);
            //All painting occurs here, between BeginPaint and EndPaint.
            EndPaint(hwnd, &ps);
            break;
        case WM_DRAWITEM:
            {
                LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT) lParam;
                if (HandleIsCurrentColorSquareHandle(lpDIS->hwndItem, CURRENT_COLOR_SQUARE_HANDLE))
                {
                    //Redraw the current color square.
                    DrawCurrentColorSquare(lpDIS, CURRENT_DRAW_COLOR, GetStockObject(DC_BRUSH), GetStockObject(DC_PEN), DEFAULT_CURRENT_COLOR_SQUARE_BORDER_COLOR);
                }
                else if (HandleIsColorPaletteOvalHandle(lpDIS->hwndItem, COLOR_PALETTE_OVAL_HANDLES, COLOR_PALETTE_OVAL_NUM))
                {
                    //Redraw a color palette oval.
                    COLORREF colorPaletteOvalColor = COLOR_PALETTE_OVAL_RGB[GetColorPaletteOvalSeqNumByHandle(lpDIS->hwndItem, COLOR_PALETTE_OVAL_HANDLES, COLOR_PALETTE_OVAL_NUM)];
                    DrawColorPaletteOval(lpDIS, colorPaletteOvalColor, GetStockObject(DC_BRUSH), GetStockObject(DC_PEN), DEFAULT_COLOR_PALETTE_OVAL_BORDER_COLOR);
                }
                else if (HandleIsDrawAreaSquareHandle(lpDIS->hwndItem, DRAW_AREA_SQUARE_HANDLES, DRAW_AREA_SQUARE_NUM))
                {
                    //Redraw a draw area square.
                    COLORREF drawAreaSquareBgColor = DRAW_AREA_SQUARE_RGB[GetDrawAreaSquareSeqNumByHandle(lpDIS->hwndItem, DRAW_AREA_SQUARE_HANDLES, DRAW_AREA_SQUARE_NUM)];
                    DrawDrawAreaSquare(lpDIS, drawAreaSquareBgColor, GetStockObject(DC_BRUSH), GetStockObject(DC_PEN), DRAW_DRAW_AREA_SQUARE_BORDER);
                } 
                else 
                return TRUE;
            }
            break;
        case WM_COMMAND:
            if (InRange(LOWORD(wParam), COLOR_PALETTE_LOW_VALUE, COLOR_PALETTE_HIGH_VALUE))
            {
                //A color palette oval has been clicked.
                HWND colorPaletteOvalHandle = GetDlgItem(hwnd, LOWORD(wParam));
                int colorPaletteOvalSeqNum = GetColorPaletteOvalSeqNumByHandle(colorPaletteOvalHandle, COLOR_PALETTE_OVAL_HANDLES, COLOR_PALETTE_OVAL_NUM);
                //Change the current draw color.
                CURRENT_DRAW_COLOR = COLOR_PALETTE_OVAL_RGB[colorPaletteOvalSeqNum];
                //Redraw the current color square with the newly selected color.
                RedrawDrawAreaSquare(CURRENT_COLOR_SQUARE_HANDLE);
            }
            else if (InRange(LOWORD(wParam), DRAW_AREA_LOW_VALUE, DRAW_AREA_HIGH_VALUE))
            {
                //A draw area square has been clicked.
                HWND drawAreaSquareHandle = GetDlgItem(hwnd, LOWORD(wParam));
                int drawAreaSquareSeqNum = GetDrawAreaSquareSeqNumByHandle(drawAreaSquareHandle, DRAW_AREA_SQUARE_HANDLES, DRAW_AREA_SQUARE_NUM);
                DRAW_AREA_SQUARE_RGB[drawAreaSquareSeqNum] = CURRENT_DRAW_COLOR;
                RedrawDrawAreaSquare(drawAreaSquareHandle);
            }

            switch (LOWORD(wParam))
            {
                //Options menu, save art.
                case ID_SAVE_ART:
                    {
                        LPWSTR bmpFileName = ShowFileSaveWindowAndGetBmpFileLocation();
                        if (bmpFileName != NULL)
                        {
                            //Convert rgb values of the draw area to bgr values.
                            FillBgrColorsOfDrawArea(DRAW_AREA_SQUARE_RGB, DRAW_AREA_SQUARE_BGR, DRAW_AREA_SQUARE_NUM);
                            //Use bgr values which represent the rgb values of the draw area to save the bmp file.
                            HBITMAP hBmpFile = GetBimapHandleOfDrawArea(DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT, DRAW_AREA_SQUARE_BGR);
                            PBITMAPINFO pBmpInfo = CreateBitmapInfoStruct(hwnd, hBmpFile);
                            CreateBitmapFile(hwnd, hBmpFile, hdc, bmpFileName, pBmpInfo);
                        }
                    }
                    break;
                //Options menu, load art from file.
                case ID_LOAD_ART:
                    {
                        LPCWSTR bmpFileName = ShowFileOpenWindowAndGetBmpFileLocation();
                        if (bmpFileName != NULL)
                        {
                            HBITMAP loadedBitmap = GetLoadedBitmapFileHandle(bmpFileName);
                            GetBitmapPixelColorData(loadedBitmap, LOADED_BMP_BGR, DRAW_AREA_SQUARE_NUM);
                            FillRgbColorsOfLoadedFile(LOADED_BMP_BGR, LOADED_BMP_RGB, DRAW_AREA_SQUARE_NUM);
                            LoadPixelColorDataIntoDrawArea(DRAW_AREA_SQUARE_RGB, LOADED_BMP_RGB);
                        }
                    }
                    break;
                //Options menu, show gridlines.
                case ID_SHOW_GRIDLINES:
                    {
                        DRAW_DRAW_AREA_SQUARE_BORDER = !DRAW_DRAW_AREA_SQUARE_BORDER;
                        UINT checkState = DRAW_DRAW_AREA_SQUARE_BORDER ? MF_CHECKED : MF_UNCHECKED;
                        CheckMenuItem(GetMenu(hwnd), ID_SHOW_GRIDLINES, checkState);
                        RedrawDrawArea();
                    }
                    break;
                default:
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0); //Send a WM_QUIT to the message queue.
            break;
        default: //For messages that we don't deal with.
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
