#include "editor.h"

SystemWindowCallback::SystemWindowCallback(Application*app)
	:
	m_app(app)
{
}

SystemWindowCallback::~SystemWindowCallback()
{
}

void SystemWindowCallback::OnSizeChanged(alSystemWindow*)
{
	m_app->OnWindowSizeChanged();
}

alVec2i SystemWindowCallback::OnGPUUpdateSize(alSystemWindow* w)
{
	alVec2i s;
	s.x = w->m_clientSize.x;
	s.y = w->m_clientSize.y;
	return s;
}

alVec2i SystemWindowCallback::OnMinMaxInfo(alSystemWindow* w)
{
	return alVec2i(800, 600);
}

void SystemWindowCallback::OnClose(alSystemWindow* window)
{
	auto windowID = window->GetID();
	if (!windowID)
	{
		m_app->m_run = false;
	}
}

void SystemWindowCallback::OnSetCursor()
{
	m_app->OnSetCursor();
}
