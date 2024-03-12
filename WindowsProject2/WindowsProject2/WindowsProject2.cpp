// WindowsProject2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject2.h"

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
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	static HDC hdc = GetDC(hWnd);
	static BOOL fDraw = FALSE;
	static POINT ptPrevious;

	static COLORREF 
		black = RGB(0, 0, 0),
		blue = RGB(0, 0, 255),
		red = RGB(255, 0, 0),
		yellow = RGB(255, 234, 255),
		currentColor = black;
	static int currentPenStyle = PS_SOLID;

	static POINT p[100];
	static int index = 0;
	
	SelectObject(hdc, CreatePen(currentPenStyle, 1, currentColor));

    switch (message)
    {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);

		DeleteObject(SelectObject(hdc, CreatePen(currentPenStyle, 1, currentColor)));

		fDraw = TRUE;
		ptPrevious.x = LOWORD(lParam);
		ptPrevious.y = HIWORD(lParam);

		Ellipse(hdc, LOWORD(lParam) - 3, HIWORD(lParam) + 3, LOWORD(lParam) + 3, HIWORD(lParam) - 3);

		ReleaseDC(hWnd, hdc);
		p[index].x = LOWORD(lParam);
		p[index++].y = HIWORD(lParam);

		break;

	case WM_MOUSEMOVE:
		if (fDraw)
		{
			hdc = GetDC(hWnd);

			DeleteObject(SelectObject(hdc, CreatePen(currentPenStyle, 1, currentColor)));

			MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL);
			LineTo(hdc, ptPrevious.x = LOWORD(lParam), ptPrevious.y = HIWORD(lParam));

			ReleaseDC(hWnd, hdc);
		}

		break;

	case WM_LBUTTONUP:
		if (fDraw)
		{
			hdc = GetDC(hWnd);

			DeleteObject(SelectObject(hdc, CreatePen(currentPenStyle, 1, currentColor)));

			MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL);
			LineTo(hdc, LOWORD(lParam), HIWORD(lParam));

			ReleaseDC(hWnd, hdc);
		}

		fDraw = FALSE;

		break; 

	case WM_LBUTTONDBLCLK:

		hdc = GetDC(hWnd);

		DeleteObject(SelectObject(hdc, CreatePen(currentPenStyle, 1, currentColor)));

		MoveToEx(hdc, p[0].x, p[0].y, NULL);

		for (int i = 1; i < index; i++) {
			LineTo(hdc, p[i].x, p[i].y);
		}
		index = 0;

		ReleaseDC(hWnd, hdc);

		break;

	case WM_RBUTTONDOWN:

		index--;

		break;
	
	case WM_CONTEXTMENU:
		{
			// Step 1: Create the main menu
			HMENU hPopupMenu = CreatePopupMenu();
			SetMenu(hWnd, hPopupMenu);

			// Step 3: Create submenus
			HMENU hMauVien = CreatePopupMenu();
			HMENU hKieuVien = CreatePopupMenu();

			// Step 2: Add menu items
			AppendMenu(hPopupMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMauVien, TEXT("Mau Vien"));
			AppendMenu(hPopupMenu, MF_STRING | MF_POPUP, (UINT_PTR)hKieuVien, TEXT("Kieu Vien"));

			// Step 4: Add items to submenus
			AppendMenu(hMauVien, MF_STRING, 0, TEXT("Black"));
			AppendMenu(hMauVien, MF_STRING, 1, TEXT("Blue"));
			AppendMenu(hMauVien, MF_STRING, 2, TEXT("Red"));
			AppendMenu(hMauVien, MF_STRING, 3, TEXT("Yellow"));

			AppendMenu(hKieuVien, MF_STRING, 4, TEXT("Solid"));
			AppendMenu(hKieuVien, MF_STRING, 5, TEXT("Dot"));
			AppendMenu(hKieuVien, MF_STRING, 6, TEXT("Dash"));

			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
		}

		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case 0:
				currentColor = black;

				break;

			case 1:
				currentColor = blue;

				break;

			case 2:
				currentColor = red;

				break;

			case 3:
				currentColor = yellow;

				break;

			case 4:
				currentPenStyle = PS_SOLID;

				break;

			case 5:
				currentPenStyle = PS_DOT;

				break;

			case 6:
				currentPenStyle = PS_DASH;

				break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

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
    case WM_DESTROY:
		DeleteObject(SelectObject(hdc, NULL));

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
