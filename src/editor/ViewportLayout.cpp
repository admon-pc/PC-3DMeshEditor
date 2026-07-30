#include "editor.h"
extern Application* g_app;
extern alMat4 g_emptyMatrix;

AppViewportLayout::AppViewportLayout()
{
}

AppViewportLayout::~AppViewportLayout()
{
	for (size_t i = 0, sz = m_viewports.size(); i < sz; ++i)
	{
		delete m_viewports[i];
	}
	for (size_t i = 0, sz = m_resizers.size(); i < sz; ++i)
	{
		delete m_resizers[i];
	}
}

void AppViewportLayout::ShowGUI()
{
	for (size_t i = 0, sz = m_viewports.size(); i < sz; ++i)
	{
		m_viewports[i]->ShowGUI();
	}
}

void AppViewportLayout::HideGUI()
{
	for (size_t i = 0, sz = m_viewports.size(); i < sz; ++i)
	{
		m_viewports[i]->HideGUI();
	}
}

AppViewport* AppViewportLayout::Add(const alVec4f& rect, AppViewportCameraType vct, AppViewportType vt)
{
	AppViewport* newViewport = new AppViewport(vt, vct, rect);
	m_activeViewport = newViewport;
	m_viewports.push_back(newViewport);
	newViewport->m_index = m_viewports.size();
	newViewport->HideGUI();
	newViewport->ResetCamera();
	newViewport->UpdateAspect();

	return newViewport;
}

void AppViewportLayout::AddResizer(AppViewportResizer* r, AppCursorType ct)
{
	m_resizers.push_back(r); 
	r->m_layout = this;
	r->m_cursorType = ct;
}

void AppViewportLayout::Rebuild()
{
	for (size_t i = 0; i < m_resizers.m_size; ++i)
	{
		m_resizers.m_data[i]->Rebuild();
	}
}

AppViewportResizer* AppViewportLayout::CursorInResizer(alInput* input)
{
	for (size_t i = 0; i < m_resizers.m_size; ++i)
	{
		if (alMath::PointInRect(input->m_cursorCoordsForGUI.x, input->m_cursorCoordsForGUI.y,
			m_resizers.m_data[i]->m_rect))
		{
			return m_resizers.m_data[i];
		}
	}

	return 0;
}

