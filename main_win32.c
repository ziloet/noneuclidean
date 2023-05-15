#include<windows.h>

#include"main_win32.h"
#include"khrplatform.h"
#include"opengl.h"

#include"opengl.c"

static GLint OcclusionCullingSupported;

static void
disable_dpi_scaling(void)
{
	HMODULE Library = LoadLibraryW(L"user32.dll");
	void* Function = GetProcAddress(Library, "SetProcessDpiAwarenessContext");
	if(Function)
	{
		typedef BOOL dpi_new_function(DPI_AWARENESS_CONTEXT);
		dpi_new_function* SetProcessDpiAwarenessContext = Function;
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		return;
	}
	Function = GetProcAddress(Library, "SetProcessDpiAware");
	if(Function)
	{
		typedef BOOL dpi_old_function(void);
		dpi_old_function* SetProcessDpiAware = Function;
		SetProcessDpiAware();
	}
}

static LRESULT CALLBACK
WindowProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
	switch(Message)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage(0);	
		} break;

		default:
		{
			Result = DefWindowProcW(Window, Message, wParam, lParam);
		}
	}
	return Result;
}

static HWND
create_window(void)
{
	HINSTANCE Instance = GetModuleHandleW(0);
	WCHAR* WindowClassName = L"wndclass_name";
	WNDCLASSEXW WindowClass = {0};
	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.lpszClassName = WindowClassName;
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = Instance;
	RegisterClassExW(&WindowClass);
	DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	RECT WindowRect = {0, 0, 1280, 720};
	AdjustWindowRect(&WindowRect, WindowStyle, 0);
	LONG WindowWidth = WindowRect.right - WindowRect.left;
	LONG WindowHeight = WindowRect.bottom - WindowRect.top;
	HWND Result = CreateWindowExW(0, WindowClassName, L"Non-Euclidian", WindowStyle, 0, 0, WindowWidth, WindowHeight, 0, 0, Instance, 0);
	Assert(IsWindow(Result));
	return Result;
}

static void
InitializeOpenGL(void)
{
	glClearColor(0.6f, 0.9f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}

static void
setup_input(HWND Window)
{
	enum
	{
		HID_USAGE_PAGE_GENERIC = 0x01,
		HID_USAGE_GENERIC_MOUSE = 0x02,
	};
	RAWINPUTDEVICE Mouse = {0};
	Mouse.usUsagePage = HID_USAGE_PAGE_GENERIC;
	Mouse.usUsage = HID_USAGE_GENERIC_MOUSE;
	Mouse.dwFlags = RIDEV_INPUTSINK;
	Mouse.hwndTarget = Window;
	RegisterRawInputDevices(&Mouse, 1, sizeof(Mouse));
}

void WinMainCRTStartup()
{
	disable_dpi_scaling();
	HWND Window = create_window();
	SetOpenGLContext(Window);
	LoadOpenGLFunctions();
	InitializeOpenGL();
	setup_input(Window);

	for(;;)
	{
		MSG Message;
		GetMessageW(&Message, 0, 0, 0);
		if(Message.message == WM_QUIT)
		{
			ExitProcess(0);
		}
		DispatchMessageW(&Message);
	}
}
