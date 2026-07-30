#pragma once
#ifndef _PCViewportL_H_
#define _PCViewportL_H_

class AppViewportResizer
{
public:
	AppViewportResizer(int id) :m_id(id) {}
	virtual ~AppViewportResizer() {}
	virtual void Rebuild() = 0;
	virtual void OnResize() = 0;

	alVec4f m_rect;
	int m_id = 0;
	AppViewportLayout* m_layout = 0;

	AppCursorType m_cursorType = AppCursorType::Arrow;

};

class AppViewportLayout
{
public:
	AppViewportLayout();
	~AppViewportLayout();

	alArray<AppViewport*> m_viewports;
	AppViewport* m_activeViewport = 0;
	
	alArray<AppViewportResizer*> m_resizers;

	void ShowGUI();
	void HideGUI();
	AppViewport* Add(const alVec4f& rect, AppViewportCameraType vct, AppViewportType vt);
	void AddResizer(AppViewportResizer* r, AppCursorType ct);
	void Rebuild();

	AppViewportResizer* CursorInResizer(alInput*);
	//void OnResize(AppViewportResizer*);
};

#endif

