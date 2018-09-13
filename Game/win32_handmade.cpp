#include <Windows.h>
#include <stdint.h>
#define internal static
#define local_persist static
#define global_variable static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

LRESULT CALLBACK MainWindowCallback
(
	HWND   window,
	UINT   message,
	WPARAM wParam,
	LPARAM lParam
);

internal void Win32ResizeDIBSection(int width, int height);
internal void Win32UpdateWindow(HDC deviceContext, RECT *clientRect, int x, int y, int width, int height);
internal void RenderWeirdGradient(int xOffset, int yOffset);


global_variable bool running;
global_variable BITMAPINFO bitmapInfo;
global_variable void *bitmapMemory;
global_variable int bitmapWidth;
global_variable int bitmapHeight;


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

	if(RegisterClass(&WindowClass))
	{
		HWND window = CreateWindowEx
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
		if(window != NULL)
		{
			running = true;
			int offsetX = 0;
			int offsetY = 0;
			while(running)
			{

				MSG Message;
				while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
				{
					if(Message.message == WM_QUIT)
					{
						running = false;
					}
					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}
				HDC deviceContext = GetDC(window);
				RECT clientRect;
				GetClientRect(window, &clientRect);
				int width = clientRect.right - clientRect.left;
				int height = clientRect.bottom - clientRect.top;

				Win32UpdateWindow(deviceContext, &clientRect, 0, 0, width, height);
				ReleaseDC(window, deviceContext);

				RenderWeirdGradient(offsetX, offsetY);
				++offsetX;
				++offsetY;
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
	switch(message)
	{
		case WM_SIZE:
		{
			RECT clientRect;
			GetClientRect(window, &clientRect);
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;
			Win32ResizeDIBSection(width, height);
			OutputDebugStringA("WM_SIZE\n");
		}
		break;

		case WM_DESTROY:
		{
			running = false;
			OutputDebugStringA("WM_DESTROY\n");
		}
		break;

		case WM_CLOSE:
		{
			running = false;
			OutputDebugStringA("WM_CLOSE\n");
		}
		break;

		case WM_PAINT:
		{
			RECT clientRect;
			GetClientRect(window, &clientRect);

			PAINTSTRUCT paint;
			HDC deviceContext = BeginPaint(window, &paint);
			int x = paint.rcPaint.left;
			int y = paint.rcPaint.top;
			int width = paint.rcPaint.right - paint.rcPaint.left;
			int height = paint.rcPaint.bottom - paint.rcPaint.top;

			Win32UpdateWindow(deviceContext, &clientRect, x, y, width, height);
			EndPaint(window, &paint);
		}

		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		}
		break;

		default:
		{
			result = DefWindowProc(window, message, wParam, lParam);
		}
		break;
	}
	return(result);
}

internal void Win32ResizeDIBSection(int width, int height)
{
	if(bitmapMemory)
	{
		VirtualFree(bitmapMemory, 0, MEM_RELEASE);
	}
	bitmapWidth = width;
	bitmapHeight = height;

	bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
	bitmapInfo.bmiHeader.biWidth = bitmapWidth;
	bitmapInfo.bmiHeader.biHeight = -bitmapHeight;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	int bytesPerPixel = 4;
	int bitmapMemorySize = (bitmapWidth*bitmapHeight)*bytesPerPixel;
	bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

internal void Win32UpdateWindow(HDC deviceContext, RECT *clientRect, int x, int y, int width, int height)
{
	int windowWidth = clientRect->right - clientRect->left;
	int windowHeight = clientRect->bottom - clientRect->top;
	StretchDIBits
	(
		deviceContext,
		0,
		0,
		bitmapWidth,
		bitmapHeight,
		0,
		0,
		windowWidth,
		windowHeight,
		/*x,
		y,
		width,
		height,
		x,
		y,
		width,
		height,*/
		bitmapMemory,
		&bitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

internal void RenderWeirdGradient(int xOffset, int yOffset)
{
	int width = bitmapWidth;
	int height = bitmapHeight;
	int bytesPerPixel = 4;
	int pitch = width * bytesPerPixel;
	uint8* row = (uint8 *)bitmapMemory;
	for(int y = 0; y < bitmapHeight; y++)
	{
		uint32 *pixel = (uint32 *)row;
		for(int x = 0; x < bitmapWidth; x++)
		{
			uint8 blue = (x + xOffset);
			uint8 green = (y + yOffset);
			*pixel++ = ((green << 8) | blue);
		}
		row += pitch;
	}
}
