#include<windows.h>

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

void WinMainCRTStartup()
{
	disable_dpi_scaling();
}
