#include "editor.h"
#include "Classes/alColor.h"

#include <Windows.h>
#include <commctrl.h>

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

AL_LINK_LIBRARY(al);

int main(int argc, char* argv[])
{
	//const char* videoDriverType = "3Editor.OpenGL33.dll"; // for example read name from .ini
	//alStringA videoDriverTypeStr = videoDriverType;
	//for (int i = 0; i < argc; ++i)
	//{
	//	if (strcmp(argv[i], "-vid") == 0)
	//	{
	//		++i;
	//		if (i < argc)
	//		{
	//			videoDriverTypeStr = argv[i];
	//		}
	//	}
	//}

	INITCOMMONCONTROLSEX iccex;
	iccex.dwICC = ICC_WIN95_CLASSES;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCommonControlsEx(&iccex);

	alLib::InitializeLib();
	Application* app = new Application;
	
	if (app->OnCreate("..."))
	{
		app->MainLoop();
	}

	delete app;

	return 1;
}
