#include "bitmap.h"

const char* className = "MyCLASSNAME!";

HWND hWndHScrollBar;

HWND hWndVScrollBar;

#define IDI_ICON       101

#define IDC_HSCROLLBAR 1000

#define IDC_VSCROLLBAR 1001

LRESULT CALLBACK WndProc( HWND    hWnd,
                          UINT    Msg,
                          WPARAM  wParam,          
                          LPARAM  lParam )
{
    static Bitmap* bitmap = new Bitmap();
    static bool canPaint = true;

    static bool init = true;

    static int pos1 = 50;
    static int pos2 = 50;
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
                "BUTTON",   // Predefined class; Unicode assumed 
                "UPDATE",       // Button text 
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
                    10,         // x position 
                        10,         // y position 
                        100,        // Button width
                            50,         // Button height
                            hWnd,       // Parent window
                                NULL,       // No menu.
                                (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
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
            canPaint = true;
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
        case WM_HSCROLL:

        {
            switch ((int)LOWORD(wParam))
            {
                case SB_LINEUP:

                    pos1 = 0;
                 
                    SetScrollPos((HWND)lParam, SB_CTL, pos1, TRUE);

                break;

                case SB_LINEDOWN:
                    
                    pos1 = 100;
                    
                    SetScrollPos((HWND)lParam, SB_CTL, pos1, TRUE);

                break;

                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                
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

                    SetScrollPos((HWND)lParam, SB_CTL, 0 , TRUE);

                break;

                case SB_LINEDOWN:
                    
                    pos2 = 0;

                    SetScrollPos((HWND)lParam, SB_CTL, 100 , TRUE);

                break;

                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    
                    pos2 = 100-HIWORD(wParam);

                    SetScrollPos((HWND)lParam, SB_CTL, HIWORD(wParam) , TRUE);

                    break;
            }

            return 0;
        }
        break;

        case WM_PAINT: {

            if(init)
            bitmap->loadBitmap("Sattelite4.bmp");

            if (canPaint) {
                bitmap->print(hWnd, pos1, pos2);

                init = false;
                canPaint = false;
            }
            break;
        }

        case WM_CLOSE:
            
            DestroyWindow(hWndVScrollBar);
            
            DestroyWindow(hWndHScrollBar);
            
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
		    "Contrast-Brightness",
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
	UpdateWindow(hWnd);

	while (GetMessage(&Msg, hWnd, 0, 0) > 0) //Get the message.
	{
		TranslateMessage(&Msg); //Translate it
		DispatchMessage(&Msg);
	}

	return Msg.wParam;

}
