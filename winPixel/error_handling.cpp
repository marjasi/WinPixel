#include <windows.h>
#include "error_handling.h"
#include "user_interface.h"

//Creates a popup with the error message in the center of the specified window.
//Returns the ID of the created error message box.
int CreateErrorMessageBox(HWND hwnd, LPCSTR errorMessage)
{
    return MessageBox(
    hwnd, //Owner window handle.
    errorMessage, //Message box message.
    TEXT("Error"), //Message box caption.
    MB_OK | MB_ICONERROR//Message box behavior and design.
    );
}
