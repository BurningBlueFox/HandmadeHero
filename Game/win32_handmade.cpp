#include <Windows.h>
#define internal static
#define local_persist static
#define global_variable static

LRESULT CALLBACK MainWindowCallback
(
	HWND   window,
	UINT   message,
	WPARAM wParam,
	LPARAM lParam
);

internal void Win32ResizeDIBSection (int width, int height);
internal void Win32UpdateWindow (HDC deviceContext, int x, int y, int width, int height);


global_variable bool running;
global_variable BITMAPINFO bitmapInfo;
global_variable void *bitmapMemory;
global_variable HBITMAP bitmapHandle;
global_variable HDC bitmapDeviceContext;


// V Entry Point V
int CALLBACK WinMain
(
	HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR     commandLine,
	int       showCode
)
{
	//MessageBox (nullptr, "HELLOOOOO", "Hi", MB_OK | MB_ICONINFORMATION);

	WNDCLASS WindowClass = {};

	WindowClass.lpfnWndProc = MainWindowCallback;
	WindowClass.hInstance = instance;
	//WindowClass.hIcon = ;
	WindowClass.lpszClassName = "HandmadeHeroWindowClass";

	if (RegisterClass (&WindowClass))
	{
		HWND WindowHandle = CreateWindowEx
		(
			0,
			WindowClass.lpszClassName,
			"Handmade Hero",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			instance,
			0
		);
		if (WindowHandle != NULL)
		{
			running = true;
			while(running)
			{
				MSG Message;
				BOOL MessageResult = GetMessage (&Message, 0, 0, 0);
				if (MessageResult > 0)
				{
					TranslateMessage (&Message);
					DispatchMessage (&Message);
				}
				else break;
			}
		}
		else
		{

		}
	}
	else
	{

	}

	return 0;
}


LRESULT CALLBACK MainWindowCallback
(
	HWND   window,
	UINT   message,
	WPARAM wParam,
	LPARAM lParam
)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_SIZE:
	{
		RECT clientRect;
		GetClientRect (window, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;
		Win32ResizeDIBSection (width, height);
		OutputDebugStringA ("WM_SIZE\n");
	}
	break;

	case WM_DESTROY:
	{
		running = false;
		OutputDebugStringA ("WM_DESTROY\n");
	}
	break;

	case WM_CLOSE:
	{
		running = false;
		OutputDebugStringA ("WM_CLOSE\n");
	}
	break;

	case WM_ACTIVATEAPP:
	{
		OutputDebugStringA ("WM_ACTIVATEAPP\n");
	}
	break;

	default:
	{
		result = DefWindowProc (window, message, wParam, lParam);
	}
	break;
	}
	return(result);
}

internal void Win32ResizeDIBSection (int width, int height) 
{
	if (bitmapHandle) 
	{
		DeleteObject (bitmapHandle);
	}
	if(bitmapDeviceContext != 0)
	{
		bitmapDeviceContext = CreateCompatibleDC (0);
	}

	bitmapInfo.bmiHeader.biSize = sizeof (bitmapInfo.bmiHeader);
	bitmapInfo.bmiHeader.biWidth = width;
	bitmapInfo.bmiHeader.biHeight = height;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	bitmapHandle = CreateDIBSection 
	(
		bitmapDeviceContext,
		&bitmapInfo,
		DIB_RGB_COLORS,
		&bitmapMemory,
		0,
		0
	);
}

internal void Win32UpdateWindow (HDC deviceContext, int x, int y, int width, int height)
{
	StretchDIBits 
	(
		deviceContext,
		x,
		y,
		width,
		height,
		x,
		y,
		width,
		height,
		bitmapMemory,
		&bitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}
