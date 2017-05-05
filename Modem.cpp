// Modem.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Modem.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void PlayHST();      
void ReceiveDTMF(); 
void PlayKOT();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MODEM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MODEM));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MODEM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MODEM);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
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
			// Play SIA Handshake tones
            PlayHST();      
			// Receive DTMF chars
            ReceiveDTMF(); 
			// Play SIA Kissof tone
            PlayKOT();

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


int err;

void PlayHST()
{
	HANDLE hComm;
	DWORD numberOfBytesWritten;
    unsigned char buffer[128];
    int numberOfBytesToRead = 128;
    DWORD numberOfBytesRead;
	DCB dcb;
	DWORD modemStat;


	hComm = CreateFile( L"COM1:", // gszPort,  
                    GENERIC_READ | GENERIC_WRITE, 
                    0, 
                    0, 
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL, // FILE_FLAG_OVERLAPPED,
                    0);
    if (hComm == INVALID_HANDLE_VALUE)
	{
        err = 1;
		return;
	} // error opening port; abort

	GetCommState(hComm, &dcb);
/*
	dcb.fOutxDsrFlow = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
*/

	SetCommState(hComm, &dcb);

	GetCommModemStatus(hComm, &modemStat);

	WriteFile(
          hComm,
          "ATE0\r\n",            // disable echo
          sizeof("ATE0\r\n"),
          &numberOfBytesWritten,
          NULL);

	// FlushFileBuffers(hComm);

	ReadFile(
          hComm,
          buffer,
          4, //numberOfBytesToRead,
          &numberOfBytesRead,
          NULL);

/*
    // Test Modem

	WriteFile(
          hComm,
          "ATDT5035452790\r\n",
          sizeof("ATDT5035452790\r\n"),
          &numberOfBytesWritten,
          NULL);

//	FlushFileBuffers(hComm);

    Sleep(2000);

	GetCommModemStatus(hComm, &modemStat);

	ReadFile(
          hComm,
          buffer,
          25, //numberOfBytesToRead,
          &numberOfBytesRead,
          NULL);

	GetCommModemStatus(hComm, &modemStat);
*/

// wait for the phone to ring

	for (;;)
	{
        GetCommModemStatus(hComm, &modemStat);

	    if (modemStat & MS_RING_ON)
		    break;

		Sleep(100);
	}

// Answer the phone

	WriteFile(
          hComm,
		  "ATA\r\n",
		  sizeof("ATA\r\n"),
          &numberOfBytesWritten,
          NULL);

// Put modem in voice mode

    WriteFile(
          hComm,
          "AT+FCLASS=8\r\n",
          sizeof("AT+FCLASS=8\r\n"),
          &numberOfBytesWritten,
          NULL);

	ReadFile(
          hComm,
          buffer,
          4, //numberOfBytesToRead,
          &numberOfBytesRead,
          NULL);

// set the inactivity timer to 10 seconds

	WriteFile(
          hComm,
		  "AT+VIT=10\r\n",
		  sizeof("AT+VIT=10\r\n"),
          &numberOfBytesWritten,
          NULL);

//  set tone duration to 100 ms

	WriteFile(
          hComm,
		  "AT+VTD=10\r\n",
		  sizeof("AT+VTD=10\r\n"),
          &numberOfBytesWritten,
          NULL);

// wait for 500 to 2000 ms

	Sleep(1000);

// Send SIA Handshake tones
//    send 1400 Hz burst

       WriteFile(
          hComm,
		  "AT+VTS=[1400]\r\n",
		  sizeof("AT+VTS=[1400]\r\n"),
          &numberOfBytesWritten,
          NULL);
/*
	   ReadFile(
          hComm,
          buffer,
          4, //numberOfBytesToRead,
          &numberOfBytesRead,
          NULL);
*/

//     pause 100 ms

	   Sleep(100);

//     send 2300 Hz burst  

       WriteFile(
          hComm,
		  "AT+VTS=[2300]\r\n",
		  sizeof("AT+VTS=[2300]\r\n"),
          &numberOfBytesWritten,
          NULL);

	   Sleep(1000);

// Hangup the phone

	WriteFile(
          hComm,
		  "ATH\r\n",
		  sizeof("ATH\r\n"),
          &numberOfBytesWritten,
          NULL);

    return;
}

void ReceiveDTMF()
{

}

void PlayKOT()
{

}
