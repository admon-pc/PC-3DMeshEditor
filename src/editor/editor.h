#pragma once
#ifndef _PCEDITOR_H_
#include "al.h"
#include "System/alCursor.h"
#include "System/alSystemWindow.h"
#include "GS/alGS.h"

class Application;

enum class AppCursorType : uint32_t
{
	Arrow,
	Cross,
	Hand,
	Help,
	IBeam,
	No,
	Size,
	SizeNESW,
	SizeNS,
	SizeNWSE,
	SizeWE,
	UpArrow,
	Wait,

	SelectObject,
	SelectVertex,
	Rotate,
	_count
};

class SystemWindowCallback : public alSystemWindowCallback
{
	Application* m_app = 0;
public:
	SystemWindowCallback(Application*);
	virtual ~SystemWindowCallback();
	virtual void OnSizeChanged(alSystemWindow*) override;
	virtual alVec2i OnGPUUpdateSize(alSystemWindow* w) override;
	virtual alVec2i OnMinMaxInfo(alSystemWindow* w) override;
	virtual void OnClose(alSystemWindow* window) override;
};


class Application
{
	friend class SystemWindowCallback;

	FILE* m_fileLog = 0;
	alCursor* m_cursors[(uint32_t)AppCursorType::_count];

	bool m_run = true;
	SystemWindowCallback* m_windowCallback = 0;
	alSystemWindow* m_mainWindow = 0;
	alGS* m_gs = 0;
public:
	Application();
	~Application();

	bool OnCreate(const char* videoDriver);
	void MainLoop();
	void PrintLog(const char* s);
};

#endif

