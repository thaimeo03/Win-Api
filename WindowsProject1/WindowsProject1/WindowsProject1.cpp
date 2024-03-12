// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject1.h"
#include<string.h>
#include<iostream>
#include<cstring>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

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
void CALLBACK		TimerGoLul(HWND, UINT, UINT, DWORD);

//		Static Values

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
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDC_ICON));

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

   SetTimer(hWnd, 0, 1000, (TIMERPROC)&TimerGoLul);

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
	static int Hinh = 0, xLeft, xRight, yTop, yBottom, Result;
	HDC hdc = GetDC(hWnd);
	POINT pt[10];

	static int currentPenStyle = PS_DOT;
	static HPEN penStyle;

	static COLORREF
		aqua = RGB(23, 255, 240),
		red = RGB(255, 0, 0),
		green = RGB(0, 255, 0),
		blue = RGB(0, 0, 255),
		black = RGB(0, 0, 0),
		currentColor = RGB(255, 0, 0);
	static int currentHatchBrush = -1;
	static HBRUSH currentBrush = CreateSolidBrush(black);

	switch (message)
	{
	case WM_LBUTTONDOWN: {
		//		Get Coord
		xLeft = LOWORD(lParam);
		yTop = HIWORD(lParam);
	}
	break;
	
	case WM_LBUTTONUP: {

		//		Get Coord
		xRight = LOWORD(lParam);
		yBottom = HIWORD(lParam);



		if (currentHatchBrush == -1) {
			DeleteObject(SelectObject(hdc, CreateSolidBrush(currentColor)));
		}
		else {
			DeleteObject(SelectObject(hdc, CreateHatchBrush(currentHatchBrush, currentColor)));
		}

		penStyle = CreatePen(currentPenStyle, 1, currentColor);
		SelectObject(hdc, penStyle);

		if (Hinh == ID_HINHHOCD_HINHCHUNHAT) {
			Rectangle(hdc, xLeft, yTop, xRight, yBottom);
		}
		else if (Hinh == ID_HINHHOCD_HINHELIP) {
			Ellipse(hdc, xLeft, yTop, xRight, yBottom);
		}
		else if (Hinh == ID_HINHHOCD_HINHTAMGIACCAN) {
			pt[0].x = xLeft + (xRight - xLeft) / 2;
			pt[0].y = yTop;
			pt[1].x = xLeft;
			pt[1].y = yBottom;
			pt[2].x = xRight;
			pt[2].y = yBottom;

			Polygon(hdc, pt, 3);
		}
		else if (Hinh == ID_HINHHOCD_LUCGIACCAN) {

			pt[0].x = xLeft + (xRight - xLeft) / 2;
			pt[0].y = yTop;
			pt[1].x = xLeft;
			pt[1].y = yTop - (yTop - yBottom) * (1.0 / 4.0);
			pt[2].x = xLeft;
			pt[2].y = yTop - (yTop - yBottom) * (3.0 / 4.0);
			pt[3].x = xLeft + (xRight - xLeft) / 2;
			pt[3].y = yBottom;
			pt[4].x = xRight;
			pt[4].y = yTop - (yTop - yBottom) * (3.0 / 4.0);
			pt[5].x = xRight;
			pt[5].y = yTop - (yTop - yBottom) * (1.0 / 4.0);

			Polygon(hdc, pt, 6);
		}
		else if (Hinh == ID_HINHHOCD_HINHTHOI) {
			pt[0].x = xLeft + (xRight - xLeft) / 2;
			pt[0].y = yTop;
			pt[1].x = xLeft;
			pt[1].y = yTop - (yTop - yBottom) / 2;
			pt[2].x = xLeft + (xRight - xLeft) / 2;
			pt[2].y = yBottom;
			pt[3].x = xRight;
			pt[3].y = yTop - (yTop - yBottom) / 2;

			Polygon(hdc, pt, 4);
		}
		else if (Hinh == ID_HINHHOCD_TAMGIACVUONG) {
			pt[0].x = xLeft;
			pt[0].y = yTop;
			pt[1].x = xLeft;
			pt[1].y = yBottom;
			pt[2].x = xRight;
			pt[2].y = yBottom;

			Polygon(hdc, pt, 3);
		}
		else if (Hinh == ID_HINHHOC_HINHCHORD) {
			pt[0].x = xLeft;
			pt[0].y = yTop;
			pt[1].x = xRight;
			pt[1].y = yBottom;
			pt[2].x = xLeft + (xRight + xLeft);
			pt[2].y = yBottom;
			pt[3].x = xLeft;
			pt[3].y = yTop;
			Chord(hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y, pt[3].x, pt[3].y);
		}
		else if (Hinh == ID_HINHHOC_HINHNGUGIAC) {
			POINT points[5];
			double radius = sqrt(pow(xRight - xLeft, 2) + pow(yBottom - yTop, 2));
			// Tính toán các góc của đỉnh (72 độ mỗi góc)
			double angleIncrement = 72 * (3.14 / 180); // Đổi sang radian
			// Tính toán tọa độ của các đỉnh
			for (int i = 0; i < 5; ++i) {
				points[i].x = xLeft + radius * cos(i * angleIncrement);
				points[i].y = yTop - radius * sin(i * angleIncrement);
			}
			Polygon(hdc, points, 5);
		}
		else if (Hinh == ID_HINHHOC_HINHTRON) {
			int radius = (int)sqrt(pow(xRight - xLeft, 2) + pow(yBottom - yTop, 2));
			Ellipse(hdc, xLeft - radius, yTop - radius, xLeft + radius, yTop + radius);
		}
	}
	break;
	
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case ID_HINHHOCD_HINHCHUNHAT:
		case ID_HINHHOCD_HINHELIP:
		case ID_HINHHOCD_HINHTAMGIACCAN:
		case ID_HINHHOCD_LUCGIACCAN:
		case ID_HINHHOCD_HINHTHOI:
		case ID_HINHHOCD_TAMGIACVUONG:
		case ID_HINHHOC_HINHCHORD:
		case ID_HINHHOC_HINHNGUGIAC:
		case ID_HINHHOC_HINHTRON:
			Hinh = wmId;

			break;

		case ID_MAUNEN_AQUA:
			currentColor = aqua;

			break;

		case ID_MAUNEN_RED:
			currentColor = red;

			break;

		case ID_MAUNEN_GREEN:
			currentColor = green;

			break;

		case ID_MAUNEN_BLUE:
			currentColor = blue;

			break;

		case ID_MAUNEN_BLACK:
			currentColor = black;

			break;

		case ID_KIEUNEN_GACHNGANG:
			currentHatchBrush = HS_HORIZONTAL;

			break;

		case ID_KIEUNEN_DANHINHTRAM:
			currentHatchBrush = HS_DIAGCROSS;

			break;

		case ID_KIEUNEN_GACHDOC:
			currentHatchBrush = HS_VERTICAL;

			break;

		case ID_KIEUNEN_DANHINHCHUTHAP:
			currentHatchBrush = HS_CROSS;

			break;

		case ID_KIEUNEN_DEFAULT:
			currentHatchBrush = -1;

			break;

		case ID_KIEUVIEN_DOT:
			currentPenStyle = PS_DOT;

			break;

		case ID_KIEUVIEN_DASH:
			currentPenStyle = PS_DASH;
			break;
		
		case ID_KIEUVIEN_SOLID:
			currentPenStyle = PS_SOLID;
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

			break;

		case ID_HINHHOCD_THOAT:
			Result = MessageBox(NULL, TEXT("You wanna quit?"), TEXT("D:"), MB_YESNO | MB_ICONQUESTION);

			if (Result == IDYES) {
				DestroyWindow(hWnd);
			}

			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		// TODO: Add any drawing code that uses hdc here...
		/*for (int i = 50; i <= 500; i++) {
			SetPixel(hdc, i, 50, RGB(255, 0, 0));
		}*/

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		DeleteObject(penStyle);
		DeleteObject(currentBrush);

		PostQuitMessage(0);

		break; 

	case WM_CLOSE:
		Result = MessageBox(NULL, TEXT("You wanna quit?"), TEXT("D:"), MB_YESNO | MB_ICONQUESTION);

		if (Result == IDYES) {
			DestroyWindow(hWnd);
		}

		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

    return 0;
}

void CALLBACK TimerGoLul(HWND hWnd, UINT uMsg, UINT timerID, DWORD dwTime)
{
	//		Initialize Timer
	static int maxTimer = 60 * 60 + 1;
	maxTimer--;
	//			Handler
	HDC hdc = GetDC(hWnd);

	//			Text Color
	SetTextColor(hdc, RGB(255, 0, 0));

	//const char temp[] = "Timer";
	//const char* tempPointer = temp;
	//LPCTSTR tempPointerLPCTSTR = (LPCTSTR)tempPointer;

	//		STRING LMAO KILL ME
	int minute = maxTimer / 60;
	int second = maxTimer % 60;
	char temp[] = "Timer: XX:XX";

	temp[7] = minute / 10 + 48;
	temp[8] = minute % 10 + 48;
	temp[10] = second / 10 + 48;
	temp[11] = second % 10 + 48;

	setlocale(LC_ALL, "");
	size_t size = strlen(temp) + 1;
	size_t len;
	wchar_t* output = (wchar_t*)malloc(size * sizeof(wchar_t));
	mbstowcs_s(&len, output, size, temp, size - 1);
	output[len] = L'\0';

	LPCTSTR temptemptemptemp = output;

	RECT rec, rect;
	int width = 210, height = 210;

	//		Location
	if (GetWindowRect(hWnd, &rect)) {
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
	
	//		DrawText
	SetRect(&rec, width - 150, height - 100, width, height);
	DrawText(hdc, temptemptemptemp, strlen(temp), &rec, DT_TOP | DT_LEFT);

	//MessageBox(NULL, TEXT("You wanna quit?"), TEXT("D:"), MB_YESNO | MB_ICONQUESTION);

	ReleaseDC(hWnd, hdc);
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
