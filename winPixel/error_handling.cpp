#include <windows.h>
#include "error_handling.h"
#include "user_interface.h"

//Creates a popup with the error message in the center of the specified window
void CreateErrorPopup(HWND hwnd, LPCSTR errorMessage, int popupWidth, int popupHeight)
{
    int centerX, centerY;

    //Find the rectangle of the specified window to calculate the center positions.
    RECT rect;
    GetWindowRect(hwnd, &rect);

    centerX = (rect.right - rect.left) / 2;
    centerY = (rect.bottom - rect.top) / 2;

    CreateWindow(
    TEXT("POPUP"), //The window is a popup.
    errorMessage, //Error message is displayed in the popup.
    WS_VISIBLE | WS_POPUP, //Styles.
    centerX, //x axis position.
    centerY, //y axis position.
    popupWidth, //Width.
    popupHeight, //Height.
    hwnd, //Parent window of the child window.
    (HMENU) ID_ERROR_POPUP, //The ID of the popup window.
    (HINSTANCE) GetWindowLong(hwnd, GWLP_HINSTANCE), //Program instance.
    NULL
    );
}
