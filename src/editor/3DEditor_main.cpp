#include "editor.h"
#include "Classes/alColor.h"
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

	alLib::InitializeLib();
	Application* app = new Application;
	
	if (app->OnCreate("..."))
	{
		app->MainLoop();
	}

	delete app;

	return 1;
}
