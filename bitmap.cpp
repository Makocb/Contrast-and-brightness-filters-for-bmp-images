#include "bitmap.h"


    Bitmap::Bitmap()
    {
        bmWidth = 0;
        bmHeight = 0;
    }

    void Bitmap::calculateBrightness(color& inpColor, const int& brightness)
    {
        auto calculate = [brightness](int& inpColor) { inpColor = inpColor * (100 + brightness - 50) / 100; };
            
        calculate(inpColor.r);

        calculate(inpColor.g);

        calculate(inpColor.b);
    }

    void Bitmap::calculateContrast(color& inpColor, const int& contrast, const double& avarageBrightness, const int& delta)
    {
        auto calculate = [contrast, delta, avarageBrightness](int& inpColor) 
        {inpColor = (int)((avarageBrightness + (delta * (100 + contrast - 50) / 100)) * inpColor / 255); };
                
        calculate(inpColor.r);

        calculate(inpColor.g);

        calculate(inpColor.b);
    }

    void Bitmap::loadBitmap(const char* name)
    {
        HDC hdc_x = CreateCompatibleDC(NULL);
        HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load the bitmap
        SelectObject(hdc_x, hBitmap); //Put the bitmap into the hdc_x

        BITMAP  bitmapInfo;
        GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);
        bmWidth = bitmapInfo.bmWidth;
        bmHeight = bitmapInfo.bmHeight;

        for (int i = 1; i <= bmWidth; i++) {
            for (int j = 1; j <= bmHeight; j++) {

                COLORREF Color = GetPixel(hdc_x, i, j);

                int r = GetRValue(Color);
                int g = GetGValue(Color);
                int b = GetBValue(Color);

                color col1 = { r,g,b };
                content.push_back(col1);
            }
        }

        DeleteDC(hdc_x);
        DeleteObject(hBitmap);
    }

    void Bitmap::print(HWND hWnd, int brightness, int contrast)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        double avarageBrightness = 0;

        RECT* rectSc1 = new RECT;

        color col1;
        COLORREF* colors = new COLORREF[bmWidth * bmHeight];

        for (int i = 1; i <= bmWidth; i++) {
            for (int j = 1; j <= bmHeight; j++) {

                col1 = content[(i - 1) * bmHeight + (j - 1)];

                calculateBrightness(col1,brightness);

                avarageBrightness += (col1.r * 0.299 + col1.g * 0.587 + col1.b * 0.114);
            }
        }

        avarageBrightness /= (bmWidth * bmHeight);

        int scale = 2;

        for (int i = 1; i <= bmWidth; i++) {
            for (int j = 1; j <= bmHeight; j++) {

                col1 = content[(i - 1) * bmHeight + (j - 1)];

                calculateBrightness(col1, brightness);

                int delta = (int)((col1.r * 0.299 + col1.g * 0.587 + col1.b * 0.114) - avarageBrightness);

                calculateContrast(col1, contrast, avarageBrightness, delta);

                auto checkBoundaries = [](int& color1)
                {
                if (color1 > 255)
                    color1 = 255; 

                if (color1 < 0)
                    color1 = 0; 
                };

                checkBoundaries(col1.r);

                checkBoundaries(col1.g);

                checkBoundaries(col1.b);

                colors[(j - 1) * (bmWidth) + (i - 1)] = RGB(col1.r, col1.g, col1.b);

            }
        }
        delete rectSc1;

        HDC hdc_x = CreateCompatibleDC(NULL);

        HBITMAP newhBitmap = CreateCompatibleBitmap(hdc_x, bmWidth, bmHeight);
        newhBitmap = CreateBitmap(bmWidth, bmHeight, 1, 32, (void*)colors);

        SelectObject(hdc_x, newhBitmap);

        RECT* rect = new RECT;
        GetWindowRect(hWnd, rect);

            int width = rect->right - rect->left;
            int height = rect->bottom - rect->top;

        StretchBlt(hdc, 0, 0, width-45, height-70, hdc_x, 0, 0, bmWidth, bmHeight, SRCCOPY);

        DeleteObject(rect);
        DeleteObject(rectSc1);

        DeleteDC(hdc);
        DeleteDC(hdc_x);

        delete[] colors;

        EndPaint(hWnd, &ps);
    }