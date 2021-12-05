#ifndef USER_INTERFACE_H_INCLUDED
#define USER_INTERFACE_H_INCLUDED

#include <windows.h>

//Top menu
#define ID_TOP_MENU 10001
#define ID_SAVE_ART 10002
#define ID_LOAD_ART 10003
#define ID_SHOW_GRIDLINES 10004

//Buttons

//Draw area squares
//IMPORTANT: DRAW_AREA_WIDTH is the number of squares in a single row
#define DRAW_AREA_WIDTH 2
//IMPORTANT: DRAW_AREA_HEIGHT is the number of squares in a single column
#define DRAW_AREA_HEIGHT 2
#define DRAW_AREA_TOP_LEFT_X 10
#define DRAW_AREA_TOP_LEFT_Y 10
#define DRAW_AREA_BOTTOM_RIGHT_X 50
#define DRAW_AREA_BOTTOM_RIGHT_Y 50
#define DRAW_AREA_SQUARE_WIDTH 20
#define DRAW_AREA_SQUARE_HEIGHT 20
//IMPORTANT: there should be enough identifiers for DRAW_AREA_WIDTH*DRAW_AREA_HEIGHT squares
#define ID_DRAW_AREA_1 30001
#define ID_DRAW_AREA_2 30002
#define ID_DRAW_AREA_3 30003
#define ID_DRAW_AREA_4 30004

#endif // USER_INTERFACE_H_INCLUDED
