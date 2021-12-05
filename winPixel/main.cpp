#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include "user_interface.h"
#include <windows.h>

void CreateDrawAreaSquare(int x, int y, int width, int height, int buttonId, HWND hwnd);
void CreateDrawArea(HWND hwnd);
void DrawDrawAreaSquare(LPARAM lParam);

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = TEXT("CodeBlocksWindowsApp");

//Constant global variables
const int DEFAULT_DRAW_AREA_SQUARE_COLOR = RGB(255, 255, 255);

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(242, 242, 242));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           TEXT("winPixel"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1280,                 /* The programs width */
           720,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           LoadMenu(hThisInstance, MAKEINTRESOURCE(ID_TOP_MENU)), /* Menu from resource file */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void CreateDrawAreaSquare(int x, int y, int width, int height, int buttonId, HWND hwnd)
{
    CreateWindow(
    TEXT("BUTTON"),  //The draw area square is a button
    TEXT("HELLO"),  //No button text
    WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, //Styles
    x, //x axis position
    y, //y axis position
    width, //Width
    height, //Height
    hwnd, //Parent window of the child window
    (HMENU) buttonId, //The ID of the button
    (HINSTANCE) GetWindowLong(hwnd, GWLP_HINSTANCE), //Program instance.
    NULL
    );
}

void CreateDrawArea(HWND hwnd)
{
    int i;
    //The total number of squares is calculated based on the width and height of the draw area
    int drawAreaSquareNum = DRAW_AREA_WIDTH*DRAW_AREA_HEIGHT;
    int currentRowNum = 1;
    int currentColumnNum = 1;
    //The first square is drawn in the top left corner
    int squareXPos = DRAW_AREA_TOP_LEFT_X;
    int squareYPos = DRAW_AREA_TOP_LEFT_Y;
    for (i = 0; i < drawAreaSquareNum; i++)
    {
        //The first square has been created. Calculate position of other squares
        if (i > 0)
        {
            //Row still not filled
            if (currentColumnNum <= DRAW_AREA_WIDTH)
            {
                squareXPos = DRAW_AREA_TOP_LEFT_X + DRAW_AREA_SQUARE_WIDTH*currentColumnNum;
                currentColumnNum++;
            }
            //New row start x position
            else
            {
                squareXPos = DRAW_AREA_TOP_LEFT_X;
                squareYPos = DRAW_AREA_TOP_LEFT_Y + DRAW_AREA_SQUARE_HEIGHT*currentRowNum;
                currentColumnNum = 1;
                currentRowNum++;
            }
        }
        CreateDrawAreaSquare(squareXPos, squareYPos, DRAW_AREA_SQUARE_WIDTH, DRAW_AREA_SQUARE_HEIGHT, ID_DRAW_AREA_1 + i, hwnd);
    }
}

void DrawDrawAreaSquare(LPARAM drawItemStructPtr, int squareColorRGB, HGDIOBJ drawingBrush)
{
    LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT) drawItemStructPtr;
    SetDCBrushColor(lpDIS -> hDC, squareColorRGB);
    SelectObject(lpDIS -> hDC, drawingBrush);
    Rectangle(lpDIS -> hDC, lpDIS -> rcItem.left, lpDIS -> rcItem.top,
        lpDIS -> rcItem.right, lpDIS -> rcItem.bottom);
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    RECT r;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            CreateDrawArea(hwnd);
            break;
        case WM_PAINT:
            PAINTSTRUCT ps;
            hdc = BeginPaint(hwnd, &ps);
            // All painting occurs here, between BeginPaint and EndPaint.
            EndPaint(hwnd, &ps);
            break;
        case WM_DRAWITEM:
            {
                DrawDrawAreaSquare(lParam, DEFAULT_DRAW_AREA_SQUARE_COLOR, GetStockObject(DC_BRUSH));
                return TRUE;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
