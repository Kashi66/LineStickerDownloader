#include <stdlib.h>  
#include <string.h>  
#include <tchar.h>
#include <windows.h>
#include "../src/Downloader.cpp"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("LineStickerDownloader");
HWND TextBox, SendButton;

int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
{
	HWND hwnd;               /* This is the handle for our window */
	MSG messages;            /* Here messages to the application are saved */
	WNDCLASSEX wincl;        /* Data structure for the windowclass */
							 /* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);
	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
											   /* Use Windows's default colour as the background of the window */
	wincl.hbrBackground = (HBRUSH)COLOR_DESKTOP;


	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 0;
	/* The class is registered, let's create the program*/
	hwnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		_T("LineStickerDownloader"),       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		440,                 /* The programs width */
		120,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);


	/* Make the window visible on the screen */
	ShowWindow(hwnd, nCmdShow);
	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
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
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)                  /* handle the messages */
	{
	case WM_CREATE:

		TextBox = CreateWindow("EDIT",
			"",
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			10, 20, 400, 20,
			hwnd, (HMENU)1, NULL, NULL);

		SendButton = CreateWindow("BUTTON",
			"Download",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			165, 45, 80, 25,
			hwnd, (HMENU)2, NULL, NULL);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 2:
			DWORD len = GetWindowTextLength(TextBox) + 1;
			LPSTR link = (LPSTR)GlobalAlloc(GPTR, len);
			GetWindowText(TextBox, link, len);
			Download(link);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}