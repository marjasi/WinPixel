#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Constant global variables
const int DRAW_AREA_SQUARE_NUM = 16*16;
const int FIRST_DRAW_AREA_SQUARE_ID = 30001;
const string DRAW_AREA_SQUARE_ID = "ID_DRAW_AREA_";


int main()
{
    int i;
    ofstream outfile("defineStatements.txt");

    for (i = 0; i < DRAW_AREA_SQUARE_NUM; i++)
    {
        outfile << "#define " << DRAW_AREA_SQUARE_ID + to_string(i + 1) << " " << FIRST_DRAW_AREA_SQUARE_ID + i << endl;
    }
    return 0;
}
