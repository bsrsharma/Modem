////////////////////////////////////////////////////////////
// Copyright 2011. Bettadapura S. Sharma                  //
//                 Portland, OR, USA                      // 
///////////////////////////////////////////////////////////
//
// HelloP.cpp : Defines the entry point for the application.
//
//#undef UNICODE

#define WINVER 0x0500						// Windows 2000 or later
#define _WIN32_WINNT 0x0500					// Windows 2000 or later

#include "stdafx.h"
#include "HelloP.h"
#include "stdio.h"

//#undef UNICODE
#include "Mmsystem.h"


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

//DWORD playWAVEfile(HWND hWndNotify, LPSTR lpszWAVEFileName);
DWORD playWAVEfile(HWND hWndNotify, LPCTSTR lpszWAVEFileName);
DWORD recordWAVEfile(DWORD dwMilliSeconds);
void DTMFdetect();
unsigned char detectTones(unsigned long start, unsigned short length);




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
	LoadString(hInstance, IDC_HELLOP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLOP));

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
}  // _tWinMain()



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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HELLOP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}  // MyRegisterClass()

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
} // InitInstance()

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
}  // WndProc()

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
/*
			playWAVEfile(hDlg, L"C:\\SIAHST.WAV");
			Sleep(1500);
			recordWAVEfile(2000);       // 2 sec, 100 ms per digit * 16 digits
            Sleep(1250);   //1.25 sec Inter-Message Time
			playWAVEfile(hDlg, L"C:\\SIAKOT.WAV");
*/
			DTMFdetect();
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}  //About()

// Plays a specified waveform-audio file using MCI_OPEN and MCI_PLAY. 
// Returns when playback begins. Returns 0L on success, otherwise 
// returns an MCI error code.

//DWORD playWAVEfile(HWND hWndNotify, LPSTR lpszWAVEfileName)
DWORD playWAVEfile(HWND hWndNotify, LPCTSTR lpszWAVEfileName)
{
    UINT wDeviceID;
    DWORD dwReturn;
    MCI_OPEN_PARMS mciOpenParms;
    MCI_PLAY_PARMS mciPlayParms;


    // Open the device by specifying the device and filename.
    // MCI will choose a device capable of playing the specified file.

//    mciOpenParms.lpstrDeviceType = "WAVEAUDIO";
    mciOpenParms.lpstrDeviceType = (LPCTSTR) L"waveaudio";
//	mciOpenParms.lpstrElementName = "C:\\SIAHST.WAV"; //lpszWAVEfileName;
    mciOpenParms.lpstrElementName = (LPCTSTR) lpszWAVEfileName;
//	mciOpenParms.lpstrAlias = "HST";
//	mciOpenParms.lpstrAlias = L"HandShakeTone";
    if (dwReturn = mciSendCommand(0, MCI_OPEN,
       MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, 
       /*(DWORD)(LPVOID)*/ (DWORD_PTR) &mciOpenParms))
    {
        // Failed to open device. Don't close it; just return error.
        return (dwReturn);
    }

    // The device opened successfully; get the device ID.
    wDeviceID = mciOpenParms.wDeviceID;

    // Begin playback. The window procedure function for the parent 
    // window will be notified with an MM_MCINOTIFY message when 
    // playback is complete. At this time, the window procedure closes 
    // the device.

    mciPlayParms.dwCallback = /*(DWORD)*/ (DWORD_PTR) hWndNotify;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY, 
        /*(DWORD)(LPVOID)*/ (DWORD_PTR) &mciPlayParms))
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
        return (dwReturn);
    }

    return (0L);
} // playWAVEfile()


DWORD recordWAVEfile(DWORD dwMilliSeconds)
{
    UINT wDeviceID;
    DWORD dwReturn;
    MCI_OPEN_PARMS mciOpenParms;
    MCI_RECORD_PARMS mciRecordParms;
    MCI_SAVE_PARMS mciSaveParms;
//  MCI_PLAY_PARMS mciPlayParms;

    // Open a waveform-audio device with a new file for recording.
    mciOpenParms.lpstrDeviceType = (LPCTSTR)L"waveaudio";
    mciOpenParms.lpstrElementName = (LPCTSTR)L"";
    if (dwReturn = mciSendCommand(0, MCI_OPEN,
        MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, 
        /*(DWORD)(LPVOID)*/ (DWORD_PTR) &mciOpenParms))
    {
        // Failed to open device; don't close it, just return error.
        return (dwReturn);
    }

    // The device opened successfully; get the device ID.
    wDeviceID = mciOpenParms.wDeviceID;

    // Begin recording and record for the specified number of 
    // milliseconds. Wait for recording to complete before continuing. 
    // Assume the default time format for the waveform-audio device 
    // (milliseconds).
    mciRecordParms.dwTo = dwMilliSeconds;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_RECORD, 
        MCI_TO | MCI_WAIT, /*(DWORD)(LPVOID)*/ (DWORD_PTR) &mciRecordParms))
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
        return (dwReturn);
    }

#ifdef ECHO_DTMF

    // Play the recording and save the file.
    mciPlayParms.dwFrom = 0L;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY,
        MCI_FROM | MCI_WAIT, /*(DWORD)(LPVOID)*/ (DWORD_PTR) &mciPlayParms))
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
        return (dwReturn);
    }

#endif

    // Save the recording to a file named DTMF.WAV. Wait for
    // the operation to complete before continuing.
    mciSaveParms.lpfilename = (LPCTSTR)L"DTMF.WAV";
    if (dwReturn = mciSendCommand(wDeviceID, MCI_SAVE,
        MCI_SAVE_FILE | MCI_WAIT, /*(DWORD)(LPVOID)*/ (DWORD_PTR) &mciSaveParms))
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
        return (dwReturn);
    }

    return (0L);
} // recordWAVEfile()

void Error(char *errmsg)
{
     printf(errmsg); 
	 printf("\n");
}

unsigned char waveData[25000];   // 8 bit PCM, 0 .. 255
//signed short waveData[25000]; // 16 bit PCM, -32768 .. 32767

// DTMFDetect--Detects DTMF symbols in a waveform-audio file.

FILE *TONES;

void DTMFdetect() 
{ 
    // char        szFileName[128];    // filename of file to open 
    HMMIO       hmmio;              // file handle for open file 
    MMCKINFO    mmckinfoParent;     // parent chunk information 
    MMCKINFO    mmckinfoSubchunk;   // subchunk information structure 
    DWORD       dwFmtSize;          // size of "FMT" chunk 
    DWORD       dwDataSize;         // size of "DATA" chunk 
    //WAVEFORMAT  *pFormat;           // address of "FMT" chunk 
	//WAVEFORMAT  format;             // Old, short struct
	struct new_waveformat_tag {
    WORD    wFormatTag;        // format type
    WORD    nChannels;         // number of channels (i.e. mono, stereo, etc.)
    DWORD   nSamplesPerSec;    // sample rate
    DWORD   nAvgBytesPerSec;   // for buffer estimation
    WORD    nBlockAlign;       // block size of data
	WORD    nBitsPerSample;    // Bits per Sample
    } newFormat;

//    HPSTR       lpData;             // address of "DATA" chunk 

		unsigned long j = 0, k = 0;
		int dat;
		unsigned char tones; // bit map of tones
		//  7        6       5      4       3      2      1      0 
		// 1209 Hz 1336 Hz 1477 Hz 1633 Hz 697 Hz 770 Hz 852 Hz 941 Hz
		unsigned char m, n, DTMF[16], i;

/*  DTMF Matrix
		
			1209 Hz 	1336 Hz 	1477 Hz 	1633 Hz
     697 Hz 	1 	      2 	      3 	      A
     770 Hz 	4 	      5 	      6 	      B
     852 Hz 	7 	      8 	      9 	      C
     941 Hz 	* 	      0 	      # 	      D
*/
		const unsigned char DTMFmatrix[4][4] =
		{'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
         
    // Open the file for reading with buffered I/O 
    // by using the default internal buffer 
    //if(!(hmmio = mmioOpen((LPWSTR)L"TONES.WAV" /*szFileName*/, NULL, 
	if(!(hmmio = mmioOpen((LPWSTR)L"DTMF.WAV" /*szFileName*/, NULL, 
        MMIO_READ | MMIO_ALLOCBUF))) 
    { 
        Error("Failed to open file."); 
        return; 
    } 
 
    // Locate a "RIFF" chunk with a "WAVE" form type to make 
    // sure the file is a waveform-audio file. 
    mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E'); 
    if (mmioDescend(hmmio, (LPMMCKINFO) &mmckinfoParent, NULL, 
        MMIO_FINDRIFF)) 
    { 
        Error("This is not a waveform-audio file."); 
        mmioClose(hmmio, 0); 
        return; 
    } 

    // Find the "FMT" chunk (form type "FMT"); it must be 
    // a subchunk of the "RIFF" chunk. 
    mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' '); 
    if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
        MMIO_FINDCHUNK)) 
    { 
        Error("Waveform-audio file has no \"FMT\" chunk."); 
        mmioClose(hmmio, 0); 
        return; 
    } 

    // Get the size of the "FMT" chunk. Allocate 
    // and lock memory for it. 
    dwFmtSize = mmckinfoSubchunk.cksize; 

	if (dwFmtSize != sizeof(newFormat))
		Error("Size Mismatch");
     
    // Read the "FMT" chunk. 
    if (mmioRead(hmmio, (HPSTR) /*pFormat*/ &newFormat, dwFmtSize) != dwFmtSize){ 
        Error("Failed to read format chunk."); 
        mmioClose(hmmio, 0); 
        return; 
    }

    // Ascend out of the "FMT" subchunk. 
    mmioAscend(hmmio, &mmckinfoSubchunk, 0); 
 
    // Find the data subchunk. The current file position should be at 
    // the beginning of the data chunk; however, you should not make 
    // this assumption. Use mmioDescend to locate the data chunk. 
    mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
    if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
        MMIO_FINDCHUNK)) 
    { 
        Error("Waveform-audio file has no data chunk."); 
        mmioClose(hmmio, 0); 
        return; 
    } 
 
    // Get the size of the data subchunk. 
    dwDataSize = mmckinfoSubchunk.cksize; 
    if (dwDataSize == 0L){ 
        Error("The data chunk contains no data."); 
        mmioClose(hmmio, 0); 
        return; 
    } 
 
    // Allocate and lock memory for the waveform-audio data. 
    // Read the waveform-audio data subchunk. 
    if(mmioRead(hmmio, (HPSTR) /*lpData*/ waveData, dwDataSize) != dwDataSize){ 
        Error("Failed to read data chunk."); 
        mmioClose(hmmio, 0); 
        return; 
    } 
 
    // Close the file. 
    mmioClose(hmmio, 0); 

// locate energy ( @ 11025 samples per sec, 50 ms = 551.25 samples)
#define PCM8
//#define PCM16

	if (fopen_s(&TONES,"TONES.OUT", "w") !=0) printf("Output file err\n");

//	fprintf_s(TONES, "Key: 1209    1336    1477    1633     697     770      852       941\n");
//	fprintf_s(TONES, "       0       1       2       3       4       5        6          7\n");


	k = 0; i = 0;
    for (; k < dwDataSize-10; )
	{
       for (; k < dwDataSize-10; k++) 
	   {
			dat = waveData[k];
#ifdef PCM8

			if (newFormat.nBitsPerSample == 8)
				dat -= 128;

#endif

#ifdef PCM16


#endif
// trim the noisy lead-in 
            if ((dat > 5) || (dat < -5))
			{
				break;
/*
// locate phase == zero before breaking out
//  if dat is positive, wait till it turns negative
				for (; k < dwDataSize - 10; k++ )
				{
					dat = waveData[k];
					if (newFormat.nBitsPerSample == 8)
     				   dat -= 128;
					if (dat < 0)
						break;
				}
// if dat is negative, wait till it turns positive
				for (; k < dwDataSize - 10; k++ )
				{
					dat = waveData[k];
					if (newFormat.nBitsPerSample == 8)
     				   dat -= 128;
					if (dat >= 0)
						break;
				}
                break; 
*/
			}
       }
       
	   if (k > dwDataSize - 560)
		   break;

       tones = detectTones(k, 500);
	   m = tones >> 4;
	   n = tones & 0xF;

	   switch (m)
	   {
		   case 1: m = 3; break;
		   case 2: m = 2; break;
		   case 4: m = 1; break;
		   case 8: m = 0; 
	   };
       switch (n)
	   {
		   case 1: n = 3; break;
		   case 2: n = 2; break;
		   case 4: n = 1; break;
		   case 8: n = 0; 
	   };

	   DTMF[i] = DTMFmatrix[n][m];

	   fprintf_s (TONES, " %c ", DTMF[i]);
	   i++;
//	   fprintf_s(TONES, "\n");
	   k += 1100; // 50 ms gap between tones
	}
    fprintf_s(TONES, "\n");
	fprintf_s(TONES, "Message:\n");
	fprintf_s(TONES, "Account number = %c%c%c%c\n", DTMF[0],DTMF[1],DTMF[2],DTMF[3]);
    fprintf_s(TONES, "Message type = %c%c\n", DTMF[4], DTMF[5]);
	fprintf_s(TONES, "Even Qualifier = %c%c%c%c\n", DTMF[6],DTMF[7],DTMF[8],DTMF[9]);
    fprintf_s(TONES, "Partition Number = %c%c\n", DTMF[10], DTMF[11]);
    fprintf_s(TONES, "Zone Number = %c%c%c\n", DTMF[12],DTMF[13],DTMF[14]);
    fprintf_s(TONES, "Checksum = %c\n", DTMF[15]);

    fclose(TONES);

	return;

}  // DTMFdetect()

#include "dtmf.cpp"   // double sinft50[554][8] data

unsigned char detectTones(unsigned long start, unsigned short length)
{
	unsigned char tonesBitMap = 0;

    unsigned char n, phase;
	unsigned short row;
	double acc[8], a;
	int wd;
	double sin, mul;
	int i;


//	fprintf_s(TONES, "start = %d ", start);

	for (n = 0; n < 8; n++)
	{
		tonesBitMap <<= 1;
        acc[n] = 0.0;
       // ACQUISITION
	   // lowest freq = 697 Hz; waveform repeats after 16 samples ( = 11025/697) 
	   // for sliding window acquisition,  start @ phase = 0 .. 16

		for (phase = 0; phase < 16; phase++)
		{
			a = 0.0;
            for (row = 0; row < length; row++)
		    {
			    wd = waveData[start+phase+row];
#ifdef PCM8
				wd -= 128;
#endif
			    sin = sinft50[row+2][n];
			    mul = wd*sin;
			    a += mul;
//fprintf_s(TONES, "%d, wd = %d, sin = %f, mul = %f, a=%f\n", row, wd, sin, mul, n, a);
		    }
            if (a < 0) a = -a;
			acc[n] += a;
	    }
//		fprintf_s(TONES, "acc[%d]=%f\n", n, acc[n]);

		i = (int) (acc[n]/40000);
//		fprintf_s(TONES, "acc[%d]=%3d , ", n, i );

		if (i > 0)
			tonesBitMap++;

	}
//	fprintf_s(TONES, "\n");

//    fprintf_s(TONES, "tones BitMap = %x\n\n", tonesBitMap);

	return tonesBitMap;

}  // detectTones()




/*
typedef unsigned long DWORD, *PDWORD, *LPDWORD;  // A DWORD is a 32-bit unsigned integer
typedef DWORD   MCIERROR;       // error return code, 0 means no error
typedef unsigned int UINT;      // A UINT is a 32-bit unsigned integer 

// __int3264 is An alias that is resolved to either:
//   An __int32 in a 32-bit translation and execution environment, or
//   An __int64 in a 64-bit translation and execution environment.
// A ULONG_PTR is an unsigned long type used for pointer precision. It is used when casting
// a pointer to a long type to perform pointer arithmetic.

typedef unsigned __int3264 ULONG_PTR; 

typedef ULONG_PTR DWORD_PTR;

// The LPSTR type and its alias PSTR specify a pointer to an array of 8-bit characters, 
// which MAY be terminated by a null character.
// In some protocols, it may be acceptable to not terminate with a null character, and this
// option will be indicated in the specification. In this case, the LPSTR or PSTR type MUST
// either be tagged with the IDL modifier [string], that indicates string semantics, or be
// accompanied by an explicit length specifier, for example [size_is()].
// The format of the characters MUST be specified by the protocol that uses them. Two common
// 8-bit formats are ANSI and UTF-8.
// A 32-bit pointer to a string of 8-bit characters, which MAY be null-terminated.
// This type is declared as follows:

typedef char* PSTR, *LPSTR; 

typedef const char* LPCSTR; 

// wchar_t : A Unicode character for use with the Microsoft Interface Definition Language
// (MIDL) compiler.

typedef unsigned short wchar_t; 

// An LPCWSTR is a 32-bit pointer to a constant string of 16-bit Unicode characters,
// which MAY be null-terminated.

typedef const wchar_t* LPCWSTR; 

#ifdef UNICODE
 typedef LPCWSTR LPCTSTR; 
#else
 typedef LPCSTR LPCTSTR;
#endif


typedef UINT    MCIDEVICEID;    // MCI device ID type

WINMMAPI MCIERROR WINAPI mciSendCommand(
    MCIDEVICEID IDDevice,
    UINT uMsg,
    DWORD_PTR fdwCommand,
    DWORD_PTR dwParam
)

typedef struct tagMCI_OPEN_PARMSW {
    DWORD_PTR   dwCallback;
    MCIDEVICEID wDeviceID;
    LPCWSTR    lpstrDeviceType;
    LPCWSTR    lpstrElementName;
    LPCWSTR    lpstrAlias;
} MCI_OPEN_PARMSW, *PMCI_OPEN_PARMSW, *LPMCI_OPEN_PARMSW;

// parameter block for MCI_PLAY command message
typedef struct tagMCI_PLAY_PARMS {
    DWORD_PTR   dwCallback;
    DWORD       dwFrom;
    DWORD       dwTo;
} MCI_PLAY_PARMS, *PMCI_PLAY_PARMS, FAR *LPMCI_PLAY_PARMS;

// MCI command message identifiers
#define MCI_OPEN                        0x0803

// flags for dwFlags parameter of MCI_OPEN command message

#define MCI_OPEN_ELEMENT                0x00000200L

#define MCI_OPEN_TYPE                   0x00002000L

#define MCIERR_BASE            256       // == 0x00000100

#define MCIERR_INVALID_DEVICE_NAME      (MCIERR_BASE + 7)  // == 0x00000107

#define MCIERR_MISSING_STRING_ARGUMENT  (MCIERR_BASE + 13) // == 0x00000113

// parameter block for MCI_RECORD command message
typedef struct tagMCI_RECORD_PARMS {
    DWORD_PTR   dwCallback;
    DWORD       dwFrom;
    DWORD       dwTo;
} MCI_RECORD_PARMS, FAR *LPMCI_RECORD_PARMS;

#define MCI_RECORD                      0x080F

// flags for dwFlags parameter of MCI_RECORD command message
#define MCI_RECORD_INSERT               0x00000100L
#define MCI_RECORD_OVERWRITE            0x00000200L

DECLARE_HANDLE(HMMIO);                  // a handle to an open file
see windef.h -- Basic Windows Type Definitions  

WINMMAPI HMMIO WINAPI mmioOpenW( IN OUT LPWSTR pszFileName, IN OUT LPMMIOINFO pmmioinfo,
                                 IN DWORD fdwOpen);

// read/write mode numbers (bit field MMIO_RWMODE)
#define MMIO_READ       0x00000000      // open file for reading only
#define MMIO_WRITE      0x00000001      // open file for writing only
#define MMIO_READWRITE  0x00000002      // open file for reading and writing

// constants for dwFlags field of MMIOINFO
#define MMIO_CREATE     0x00001000      // create new file (or truncate file)
#define MMIO_PARSE      0x00000100      // parse new file returning path
#define MMIO_DELETE     0x00000200      // create new file (or truncate file)
#define MMIO_EXIST      0x00004000      // checks for existence of file
#define MMIO_ALLOCBUF   0x00010000      // mmioOpen() should allocate a buffer
#define MMIO_GETTEMP    0x00020000      // mmioOpen() should retrieve temp name

// various MMIO flags
#define MMIO_FHOPEN             0x0010  // mmioClose: keep file handle open
#define MMIO_EMPTYBUF           0x0010  // mmioFlush: empty the I/O buffer
#define MMIO_TOUPPER            0x0010  // mmioStringToFOURCC: to u-case
#define MMIO_INSTALLPROC    0x00010000  // mmioInstallIOProc: install MMIOProc
#define MMIO_GLOBALPROC     0x10000000  // mmioInstallIOProc: install globally
#define MMIO_REMOVEPROC     0x00020000  // mmioInstallIOProc: remove MMIOProc
#define MMIO_UNICODEPROC    0x01000000  // mmioInstallIOProc: Unicode MMIOProc
#define MMIO_FINDPROC       0x00040000  // mmioInstallIOProc: find an MMIOProc
#define MMIO_FINDCHUNK          0x0010  // mmioDescend: find a chunk by ID
#define MMIO_FINDRIFF           0x0020  // mmioDescend: find a LIST chunk
#define MMIO_FINDLIST           0x0040  // mmioDescend: find a RIFF chunk
#define MMIO_CREATERIFF         0x0020  // mmioCreateChunk: make a LIST chunk
#define MMIO_CREATELIST         0x0040  // mmioCreateChunk: make a RIFF chunk

WINMMAPI FOURCC WINAPI mmioStringToFOURCCW( IN LPCWSTR sz, IN UINT uFlags);

typedef DWORD           FOURCC;         // a four character code

// RIFF chunk information data structure
typedef struct _MMCKINFO
{
        FOURCC          ckid;           // chunk ID
        DWORD           cksize;         // chunk size
        FOURCC          fccType;        // form type or list type
        DWORD           dwDataOffset;   // offset of data portion of chunk
        DWORD           dwFlags;        // flags used by MMIO functions
} MMCKINFO, *PMMCKINFO, NEAR *NPMMCKINFO, FAR *LPMMCKINFO;

WINMMAPI MMRESULT WINAPI mmioDescend( IN HMMIO hmmio, IN OUT LPMMCKINFO pmmcki,
    IN const MMCKINFO FAR* pmmckiParent, IN UINT fuDescend);

// OLD general waveform format structure (information common to all formats)
typedef struct waveformat_tag {
    WORD    wFormatTag;        // format type
    WORD    nChannels;         // number of channels (i.e. mono, stereo, etc.)
    DWORD   nSamplesPerSec;    // sample rate
    DWORD   nAvgBytesPerSec;   // for buffer estimation
    WORD    nBlockAlign;       // block size of data
} WAVEFORMAT, *PWAVEFORMAT, NEAR *NPWAVEFORMAT, FAR *LPWAVEFORMAT;

WINMMAPI LONG WINAPI mmioRead( IN HMMIO hmmio, OUT HPSTR pch, IN LONG cch);

8-bit samples are stored as unsigned bytes, ranging from 0 to 255.
16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767.


*/






