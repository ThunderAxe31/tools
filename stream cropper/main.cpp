#if defined(UNICODE) && !defined(_UNICODE)
	#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
	#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpszArgument,
					int nCmdShow)
{
	HWND hwnd;				/* This is the handle for our window */
	MSG messages;			/* Here messages to the application are saved */
	WNDCLASSEX wincl;		/* Data structure for the windowclass */

	/* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;	/* This function is called by windows */
	wincl.style = CS_DBLCLKS;				/* Catch double-clicks */
	wincl.cbSize = sizeof (WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;				/* No menu */
	wincl.cbClsExtra = 0;					/* No extra bytes after the window class */
	wincl.cbWndExtra = 0;					/* structure or the window instance */
	/* Use Windows's default colour as the background of the window */
	//wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(0, 0, 0));

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx (&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	hwnd = CreateWindowEx (
			0,					/* Extended possibilites for variation */
			szClassName,		/* Classname */
			_T("1280x720@60fps Screen"),	   /* Title Text */
			WS_POPUP, /* default window */
			0,		//x pos
			0,		//y pos
			1280,				//width
			720,				//height
			HWND_DESKTOP,		/* The window is a child-window to desktop */
			NULL,				/* No menu */
			hThisInstance,		/* Program Instance handler */
			NULL				/* No Window Creation data */
			);

	/* Make the window visible on the screen */
	ShowWindow (hwnd, nCmdShow);

	UINT_PTR  IDT_TIMER1;
	SetTimer(hwnd,			// handle to main window
	IDT_TIMER1,				// timer identifier
	16,		//60 FPS
//	32,		//30 FPS
//	66,		//15 FPS
	(TIMERPROC) NULL);		// no timer callback

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage (&messages, NULL, 0, 0))
	{
	
		/* Translate virtual-key messages into character messages */
		TranslateMessage(&messages);
		/* Send message to WindowProcedure */
		DispatchMessage(&messages);
	}

	/* The program return-value is 0 - The value that PostQuitMessage() gave */
	return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)				/* handle the messages */
	{
		case WM_TIMER:
			RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE | RDW_INTERNALPAINT);
			break;
		case WM_PAINT:{
			PAINTSTRUCT	 ps;

			HDC hdc = BeginPaint(hwnd, &ps);

	// copy screen to bitmap
	HDC	 hScreen = GetDC(NULL);
	BitBlt(hdc, 0, 0, 1280, 720, hScreen, 0, 0, SRCCOPY);

			EndPaint(hwnd, &ps);

	// clean up
	ReleaseDC(NULL, hScreen);

			break;
			}
		case WM_DESTROY:
			PostQuitMessage (0);		/* send a WM_QUIT to the message queue */
			break;
		default:					/* for messages that we don't deal with */
			return DefWindowProc (hwnd, message, wParam, lParam);
	}

	return 0;
}
