
#include <windows.h>
#include <cmath>
#include <commctrl.h>
#include <wingdi.h>
#include <windef.h>
using namespace std;

const char* className = "MyCLASSNAME!";

HWND hWndHScrollBar;

HWND hWndVScrollBar;

#define IDI_ICON       101

#define IDC_HSCROLLBAR 1000

#define IDC_VSCROLLBAR 1001



//HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, "Sattelite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

LRESULT CALLBACK WndProc( HWND    hWnd,
                          UINT    Msg,
                          WPARAM  wParam,          
                          LPARAM  lParam )
{
    static int pos1 = 50;
    static int pos2 = 50;

    static bool pos1Changed = TRUE;
    static bool pos2Changed = TRUE;
    switch (Msg)
    {


        case WM_CREATE:
        {


            int ScrollBarWidth  = 25;
            int ScrollBarHeight = 15;
            
            RECT Rect;
            GetClientRect(hWnd, &Rect);
 
            hWndVScrollBar = CreateWindowEx(
            0,
            "SCROLLBAR", 
            NULL, 
            WS_VISIBLE | WS_CHILD | SBS_VERT,
            Rect.right - ScrollBarWidth, 
            0, 
            ScrollBarWidth, 
            Rect.bottom - ScrollBarHeight, 
            hWnd, 
            (HMENU)IDC_VSCROLLBAR, 
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
            NULL);

            HWND hwndButton = CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed 
                "OK",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
                10,         // x position 
                10,         // y position 
                100,        // Button width
                50,        // Button height
                hWnd,     // Parent window
                NULL,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.

            if (!hWndVScrollBar)

                MessageBox(NULL, "Vertical Scroll Bar Failed.", "Error", MB_OK | MB_ICONERROR);

            hWndHScrollBar = CreateWindowEx(
            0,
            "SCROLLBAR", 
            NULL, 
            WS_VISIBLE | WS_CHILD | SBS_HORZ,
            0,
            Rect.bottom - ScrollBarHeight, 
            Rect.right - ScrollBarWidth, 
            ScrollBarHeight, 
            hWnd, 
            (HMENU)IDC_HSCROLLBAR, 
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
            NULL);



            if (!hWndHScrollBar)
                MessageBox(NULL, "Horizontal Scroll Bar Failed.", "Error", MB_OK | MB_ICONERROR);

            SetScrollRange(hWndVScrollBar, SB_CTL, 0, 100, TRUE);
            SetScrollPos(hWndVScrollBar, SB_CTL, 50, TRUE);

            SetScrollRange(hWndHScrollBar, SB_CTL, 0, 100, TRUE);
            SetScrollPos(hWndHScrollBar, SB_CTL, 50, TRUE);

        }

        break; 
        case  WM_COMMAND:
        {
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
        case WM_HSCROLL:

        {
            switch ((int)LOWORD(wParam))
            {
                case SB_LINEUP:
                    pos1 = 0;
                    pos1Changed = true;
                    SetScrollPos((HWND)lParam, SB_CTL, pos1, TRUE);
                break;

                case SB_LINEDOWN:
                    pos1Changed = true;
                    pos1 = 100;
                    SetScrollPos((HWND)lParam, SB_CTL, pos1, TRUE);
                break;
                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    pos1Changed = true;
                    pos1 = HIWORD(wParam);
                    SetScrollPos((HWND)lParam, SB_CTL, pos1, TRUE);
                    break;
            }
           

            return 0;

        }
        break;

        case WM_VSCROLL:
        {
            switch ((int)LOWORD(wParam))

            {
                case SB_LINEUP:
                    pos2 = 100;
                    pos2Changed = true;
                    SetScrollPos((HWND)lParam, SB_CTL, HIWORD(wParam) , TRUE);
                break;

                case SB_LINEDOWN:
                    pos2Changed = true;
                    pos2 = 0;
                    SetScrollPos((HWND)lParam, SB_CTL, HIWORD(wParam) , TRUE);
                break;

                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    pos2Changed = true;
                    pos2 = 100-HIWORD(wParam);
                    SetScrollPos((HWND)lParam, SB_CTL, HIWORD(wParam) , TRUE);
                    break;
            }

            return 0;
        }
        break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HDC hdc_x = CreateCompatibleDC(NULL);
            HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, "Sattelite2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load the bitmap
            SelectObject(hdc_x, hBitmap); //Put the bitmap into the hdc_x

            BITMAP  bitmapInfo;
            GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);
            int bmWidth = bitmapInfo.bmWidth;
            int bmHeight = bitmapInfo.bmHeight;


            double avarageBrightness=0;
            RECT* rectSc1 = new RECT;
            //RECT rectSc;
            if (pos2Changed || pos1Changed) {
                for (int i = 1; i <= bmWidth; i++) {
                    for (int j = 1; j <= bmHeight; j++) {
                        COLORREF Color = GetPixel(hdc_x, i, j);
                        int r = GetRValue(Color) * (100 + pos1 - 50) / 100;
                        int g = GetGValue(Color) * (100 + pos1 - 50) / 100;
                        int b = GetBValue(Color) * (100 + pos1 - 50) / 100;
                        avarageBrightness += (r * 0.299 + g * 0.587 + b * 0.114);
                    }
                }
            }
            avarageBrightness /= (bmWidth * bmHeight);

            int scale = 2;
            if (pos2Changed|| pos1Changed)
            for (int i = 1; i <= bmWidth; i++) {
                for (int j = 1; j <= bmHeight; j++) {
                    COLORREF Color = GetPixel(hdc_x, i, j);
                  
                    int r = GetRValue(Color) * (100+pos1-50)/100;
                    int g = GetGValue(Color) * (100+pos1-50)/100;
                    int b = GetBValue(Color) * (100+pos1-50)/100;
                    double Brightness = (r * 0.299 + g * 0.587 + b * 0.114);
                    int delta = (int)(Brightness-avarageBrightness);
                    
                    //if (100 + pos2 - 50)!=0)
                    r = (int)((avarageBrightness + (delta* (100 + pos2 - 50) / 100))*r/255);
                    g = (int)((avarageBrightness + (delta * (100 + pos2 - 50) / 100))*g/255);
                    b = (int)((avarageBrightness + (delta * (100 + pos2 - 50) / 100))*b/255);
                    if (r > 255) { r = 255; };
                    if (g > 255) { g = 255; };
                    if (b > 255) { b = 255; };
                    if (r < 0) { r = 0; };
                    if (g < 0) { g = 0; };
                    if (b < 0) { b = 0; };
                    COLORREF clrref = RGB(r, g, b);
                    //(i + (scale - 1) * (i - 200), j + (scale - 1) * (j - 1), i + (scale - 1) * (i - 199), j + (scale - 1) * (j));
                    rectSc1->left = i+200 + (scale-1) * (i-(scale-1));
                    rectSc1->top = j + (scale-1) * (j - (scale-1));
                    rectSc1->right = i+200 + (scale-1) * (i)+1;
                    rectSc1->bottom = j + (scale-1) * (j)+1;
                    HBRUSH hBrush = CreateSolidBrush(clrref);
                    FillRect(hdc, rectSc1, hBrush);
                    DeleteObject(hBrush);


                    //for (int k = 0; k <= scale-1; k++)
                    //for (int m = 0; m <= scale-1; m++)
                    //COLORREF a = SetPixel(hdc_x, i+(scale-1)*(i-200)+k, j+m+ (scale-1) * (j-1), clrref);
                }
            }
            delete rectSc1;


            RECT rect;
            GetWindowRect(hWnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            //StretchBlt(hdc, 0, 0, width, height, hdc_x,100,200,4861,3308, SRCCOPY); //Draw it.

            EndPaint(hWnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hWnd);
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
        break;

        default:
            return (DefWindowProc(hWnd, Msg, wParam, lParam));

    }



    return 0;

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//the (int main) for the WINAPI programs.
    WNDCLASSEXA wc = {0};
	HWND hWnd;
    MSG Msg = {0};

	wc.cbSize = sizeof(WNDCLASSEX);
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); //The background color of the window, you can modify it.
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;//CreateSolidBrush(RGB(/*RGB: Red, Green, Blue.*/ 200, 200, 200));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //The cursor
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //The icon
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc; //The window procedure!
	wc.lpszClassName = className;
	wc.lpszMenuName = "MENU!";
	wc.style = CS_HREDRAW | CS_VREDRAW;; //The window style.

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "No window.", "Uh oh...", MB_ICONERROR | MB_OK);
		return -1;
	}

	//Create the window
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		"Scroll Bars",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		520,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, "No window.", "Uh oh...", MB_ICONERROR | MB_OK);
		return -1; //Cancel window creation.
	}

	ShowWindow(hWnd, nShowCmd);
    //RedrawWindow(hWnd, NULL, NULL, NULL);
	UpdateWindow(hWnd);

	while (GetMessage(&Msg, hWnd, NULL, NULL)) //Get the message.
	{
		TranslateMessage(&Msg); //Translate it
		DispatchMessage(&Msg);
        //InvalidateRect(hWnd, NULL, true);

        //RedrawWindow(hWnd, NULL, NULL, NULL);
        UpdateWindow(hWnd);
        
	}

	return Msg.wParam;

}
