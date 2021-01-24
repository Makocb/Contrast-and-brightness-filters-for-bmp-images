#include "bitmap.h"

const char* className = "MyCLASSNAME!";


HWND hwndTrackVr;

HWND hwndTrackHr;

#define IDI_ICON       101

#define IDC_HSCROLLBAR 1000

#define IDC_VSCROLLBAR 1001

#define ID_TRACKBAR 1002

LRESULT CALLBACK WndProc( HWND    hWnd,
                          UINT    Msg,
                          WPARAM  wParam,          
                          LPARAM  lParam )
{

    static Bitmap* bitmap = new Bitmap();
    static bool canPaint = true;

    static bool init = true;

    static int pos1 = 0;
    static int pos2 = 100;
    switch (Msg)
    {


        case WM_CREATE:
        {

            int ScrollBarWidth  = 25;
            int ScrollBarHeight = 25;
            
            RECT Rect;
            GetClientRect(hWnd, &Rect);

            hwndTrackVr = CreateWindowEx(
                0,                                      // no extended styles 
                TRACKBAR_CLASS,                         // class name 
                "TrackBar1",                            // title (caption) 
                WS_CHILD |
                WS_VISIBLE |
                TBS_AUTOTICKS |
                TBS_ENABLESELRANGE | TBS_VERT,          // style 
                Rect.right - ScrollBarWidth,
                0,
                ScrollBarWidth,
                Rect.bottom - ScrollBarHeight,          // size 
                hWnd,                                   // parent window 
                (HMENU)ID_TRACKBAR,                     // control identifier 
                (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),                   
                NULL                                    // no WM_CREATE parameter 
            );

            hwndTrackHr = CreateWindowEx(
                0,                                      // no extended styles 
                TRACKBAR_CLASS,                         // class name 
                "TrackBar2",              
                WS_CHILD | 
                WS_VISIBLE | 
                TBS_AUTOTICKS | 
                TBS_ENABLESELRANGE| TBS_HORZ,           // style 
                0, Rect.bottom - ScrollBarHeight,       // position 
                Rect.right - ScrollBarWidth,
                ScrollBarHeight,                        // size 
                hWnd,                                   // parent window 
                (HMENU)ID_TRACKBAR,                     // control identifier 
                (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),                      
             NULL                                       // no WM_CREATE parameter 
             ); 


            SendMessageW(hwndTrackHr, TBM_SETPOS, FALSE, 0);
            SendMessageW(hwndTrackVr, TBM_SETPOS, FALSE, 0);

        }

        break; 

        case WM_HSCROLL:

        {
            switch ((int)LOWORD(wParam))
            {

                case TB_ENDTRACK:
                {
                    canPaint = true;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
                break;

                case TB_THUMBPOSITION:
                case TB_THUMBTRACK:
                {
                    pos1 = HIWORD(wParam);
                }
                    break;
            }
           
            return 0;

        }

        break;

        case WM_VSCROLL:
        {
            switch ((int)LOWORD(wParam))

            {

            case TB_ENDTRACK:
            {
                canPaint = true;
                InvalidateRect(hWnd, NULL, FALSE);
            }

            break;

                case TB_THUMBPOSITION:
                case TB_THUMBTRACK:
                {
                    pos2 = 100 - HIWORD(wParam);
                }
                    break;
            }

            return 0;
        }
        break;


        case WM_PAINT: {

            if(init)
            bitmap->loadBitmap("Sattelite.bmp");

            if (canPaint) {
                bitmap->print(hWnd, pos1, pos2);

                init = false;
                canPaint = false;
            }
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
