// WindowsProject5.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject5.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT5));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT5);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, TEXT("Vẽ Hình"), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd, 0, 1000, NULL);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int kieuHinh, kieuVien, result;
	static COLORREF
		mauDen = RGB(0, 0, 0),
		mauVang = RGB(255, 255, 0),
		mauDo = RGB(255, 0, 0),
		kieuMau = mauDen;
	static TCHAR timeLeft[16];
	static int m = 60, s = 0;
	static int xLeft, xRight, yTop, yBottom;
	POINT pt[3];

	HDC hdc;

    switch (message)
    {
	case WM_TIMER:
		if (s > 0) {
			s--;
		}
		else {
			if (m > 0) {
				m--;
				s = 59;
			}
			else {
				KillTimer(hWnd, 1);
			}
		}

		wsprintfW(timeLeft, L"Time Left: %d:%d", s, m);

		hdc = GetDC(hWnd);

		SetTextColor(hdc, mauDo);

		RECT rect;
		int width, height;

		if (GetWindowRect(hWnd, &rect)) {
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}

		TextOut(hdc, width - 150, height - 100, timeLeft, 16);

		ReleaseDC(hWnd, hdc);

		break;

	case WM_LBUTTONDOWN:
		xLeft = LOWORD(lParam);
		yTop = HIWORD(lParam);

		break;

	case WM_LBUTTONUP:
		xRight = LOWORD(lParam);
		yBottom = HIWORD(lParam);

		hdc = GetDC(hWnd);

		DeleteObject(SelectObject(hdc, CreatePen(kieuVien, 3, kieuMau)));

		if (kieuHinh == ID_HINH_HINHCHORD) {
			Chord(hdc, xLeft, yTop, xRight, yBottom, (xLeft + xRight) / 2, yTop, xRight, (yTop + yBottom) / 2);
		}
		else if (kieuHinh == ID_HINH_TAMGIACVUONG) {
			pt[0].x = xLeft;
			pt[0].y = yTop;
			pt[1].x = xRight;
			pt[1].y = yTop;
			pt[2].x = xRight;
			pt[2].y = yBottom;

			Polygon(hdc, pt, 3);
		}
		else if (kieuHinh == ID_HINH_HINHCHUNHAT) {
			Rectangle(hdc, xLeft, yTop, xRight, yBottom);
		}

		ReleaseDC(hWnd, hdc);

		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_HINH_HINHCHORD:
			case ID_HINH_HINHCHUNHAT:
			case ID_HINH_TAMGIACVUONG:

				kieuHinh = wmId;

				break;

			case ID_KIEUVIEN_:
				kieuVien = PS_SOLID;

				break;

			case ID_KIEUVIEN_32777:
				kieuVien = PS_DOT;

				break;

			case ID_HINH_THOAT:
				result = MessageBox(NULL, TEXT("You wanna quit?"), TEXT("D:"), MB_YESNO | MB_ICONQUESTION);

				if (result == IDYES) {
					DestroyWindow(hWnd);
				}

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CLOSE:

		result = MessageBox(NULL, TEXT("You wanna quit?"), TEXT("D:"), MB_YESNO | MB_ICONQUESTION);

		if (result == IDYES) {
			DestroyWindow(hWnd);
		}

		break;
    case WM_DESTROY:
        PostQuitMessage(0);

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
