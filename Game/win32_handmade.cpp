#include <Windows.h>
LRESULT CALLBACK MainWindowCallback
(
	HWND   Window,
	UINT   Message,
	WPARAM WParam,
	LPARAM LParam
);



// V Entry Point V
int CALLBACK WinMain
(
	HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR     CommandLine,
	int       ShowCode
)
{
	//MessageBox (nullptr, "HELLOOOOO", "Hi", MB_OK | MB_ICONINFORMATION);

	WNDCLASS WindowClass = {};

	WindowClass.lpfnWndProc = MainWindowCallback;
	WindowClass.hInstance = Instance;
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
			Instance,
			0
		);
		if (WindowHandle != NULL)
		{
			for (;;)
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
	HWND   Window,
	UINT   Message,
	WPARAM WParam,
	LPARAM LParam
)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_SIZE:
	{
		OutputDebugStringA ("WM_SIZE\n");
	}
	break;

	case WM_DESTROY:
	{
		OutputDebugStringA ("WM_DESTROY\n");
	}
	break;

	case WM_CLOSE:
	{
		PostQuitMessage (0);
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
		Result = DefWindowProc (Window, Message, WParam, LParam);
	}
	break;
	}
	return(Result);
}
