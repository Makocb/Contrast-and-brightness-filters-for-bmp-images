#include "bitmap.h"


    Bitmap::Bitmap()
    {
        bmWidth = 0;
        bmHeight = 0;
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

    void Bitmap::print(HWND hWnd, int pos1, int pos2)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        double avarageBrightness = 0;

        RECT* rectSc1 = new RECT;

        color col1;

        for (int i = 1; i <= this->bmWidth; i++) {
            for (int j = 1; j <= this->bmHeight; j++) {

                col1.r = this->content[(i - 1) * (this->bmHeight) + (j - 1)].r * (100 + pos1 - 50) / 100;
                col1.g = this->content[(i - 1) * (this->bmHeight) + (j - 1)].g * (100 + pos1 - 50) / 100;
                col1.b = this->content[(i - 1) * (this->bmHeight) + (j - 1)].b * (100 + pos1 - 50) / 100;

                avarageBrightness += (col1.r * 0.299 + col1.g * 0.587 + col1.b * 0.114);
            }
        }

        avarageBrightness /= (this->bmWidth * this->bmHeight);

        int scale = 2;

        for (int i = 1; i <= this->bmWidth; i++) {
            for (int j = 1; j <= this->bmHeight; j++) {

                col1.r = this->content[(i - 1) * (this->bmHeight) + (j - 1)].r * (100 + pos1 - 50) / 100;
                col1.g = this->content[(i - 1) * (this->bmHeight) + (j - 1)].g * (100 + pos1 - 50) / 100;
                col1.b = this->content[(i - 1) * (this->bmHeight) + (j - 1)].b * (100 + pos1 - 50) / 100;

                int delta = (int)((col1.r * 0.299 + col1.g * 0.587 + col1.b * 0.114) - avarageBrightness);

                col1.r = (int)((avarageBrightness + (delta * (100 + pos2 - 50) / 100)) * col1.r / 255);
                col1.g = (int)((avarageBrightness + (delta * (100 + pos2 - 50) / 100)) * col1.g / 255);
                col1.b = (int)((avarageBrightness + (delta * (100 + pos2 - 50) / 100)) * col1.b / 255);

                if (col1.r > 255) { col1.r = 255; };
                if (col1.g > 255) { col1.g = 255; };
                if (col1.b > 255) { col1.b = 255; };
                if (col1.r < 0) { col1.r = 0; };
                if (col1.g < 0) { col1.g = 0; };
                if (col1.b < 0) { col1.b = 0; };

                COLORREF clrref = RGB(col1.r, col1.g, col1.b);

                rectSc1->left = i + 200 + (scale - 1) * (i - (scale - 1));
                rectSc1->top = j + (scale - 1) * (j - (scale - 1));
                rectSc1->right = i + 200 + (scale - 1) * (i)+1;
                rectSc1->bottom = j + (scale - 1) * (j)+1;
                HBRUSH hBrush = CreateSolidBrush(clrref);
                FillRect(hdc, rectSc1, hBrush);
                DeleteObject(hBrush);

                //for (int k = 0; k <= scale-1; k++)
                //for (int m = 0; m <= scale-1; m++)
                //COLORREF a = SetPixel(hdc_x, i+(scale-1)*(i-200)+k, j+m+ (scale-1) * (j-1), clrref);
            }
        }
        delete rectSc1;

        DeleteObject(rectSc1);
        DeleteDC(hdc);

        EndPaint(hWnd, &ps);
    }