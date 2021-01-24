#pragma once

#include <vector>
#include <windows.h>
#include <cmath>
#include <commctrl.h>
#include <wingdi.h>
//#include <windef.h>

struct color
{
    int r, g, b;
};

class Bitmap
{
public:
    std::vector<color> content;

    int bmWidth;
    int bmHeight;

    Bitmap();

    void calculateBrightness(color& inpColor, const int& brightness);

    void calculateContrast(color& inpColor,const int& contrast, const double& avarageBrightness, const int& delta);

    void loadBitmap(const char* name);

    void print(HWND hWnd, int pos1, int pos2);
};

