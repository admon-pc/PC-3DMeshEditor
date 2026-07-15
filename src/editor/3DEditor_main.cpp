#include "editor.h"
AL_LINK_LIBRARY(al);

Application* g_app = 0;

void PrintLogFunction(const char*s)
{
	g_app->PrintLog(s);
}

Application::Application()
{
	g_app = this;
}

Application::~Application()
{
	AL_DESTROY(m_windowCallback);

	if (m_fileLog)
		fclose(m_fileLog);
}

bool Application::OnCreate(const char* videoDriver)
{
	alLog::SetPrintFunction(PrintLogFunction);
	for (uint32_t i = 0; i < (uint32_t)AppCursorType::_count; ++i)
	{
		switch ((AppCursorType)i)
		{
		case AppCursorType::Arrow: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/arrow.cur"); break;
		case AppCursorType::Cross: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/prec.cur"); break;
		case AppCursorType::Hand: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/link.cur"); break;
		case AppCursorType::Help: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/helpsel.cur"); break;
		case AppCursorType::IBeam: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/select.cur"); break;
		case AppCursorType::No: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/unavail.cur"); break;
		case AppCursorType::Size: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/move.cur"); break;
		case AppCursorType::SizeNESW: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/nesw.cur"); break;
		case AppCursorType::SizeNS: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/ns.cur"); break;
		case AppCursorType::SizeNWSE: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/nwse.cur"); break;
		case AppCursorType::SizeWE: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/ew.cur"); break;
		case AppCursorType::UpArrow: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/up.cur"); break;
		case AppCursorType::Wait: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/working.ani"); break;
		case AppCursorType::SelectObject: m_cursors[i] = alLib::CreateCursor(alCursorType::Arrow, "../data/cursors/aero-no-tail/prec.cur"); break;
		case AppCursorType::SelectVertex: m_cursors[i] = alLib::CreateCursor(alCursorType::Arrow, "../data/cursors/aero-no-tail/prec.cur"); break;
		case AppCursorType::Rotate: m_cursors[i] = alLib::CreateCursor(alCursorType::Arrow, "../data/cursors/rotate.cur"); break;
		default:break;
		}

		if(i < (uint32_t)alCursorType::_count)
			alLib::SetCursor((alCursorType)i, m_cursors[i]);
	}
	alLib::SetCursor(alCursorType::Arrow, m_cursors[(uint32_t)AppCursorType::Arrow]);

	m_windowCallback = alCreate<SystemWindowCallback>(this);
	m_mainWindow = alLib::CreateSystemWindow(m_windowCallback);
	if (!m_mainWindow)
		return false;
	m_mainWindow->Show();

	m_gs = alLib::CreateGS(alVideoDriverType::Direct3D11);
	if (!m_gs->Init(m_mainWindow))
		return false;
}

void Application::MainLoop()
{
	while (m_run)
	{
		alLib::Update();
	}
}

void Application::PrintLog(const char* s)
{
	if (!m_fileLog)
	{
		fopen_s(&m_fileLog, "log.txt", "wb");
	}

	printf("%s", s);

	if (m_fileLog)
	{
		fprintf(m_fileLog, "%s", s);
	}
}

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
	
	app->OnCreate("...");
	app->MainLoop();

	delete app;

	return 1;
}
