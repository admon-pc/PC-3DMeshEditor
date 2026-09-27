#include "editor.h"

extern Application* g_app;

AppOStream::AppOStream()
{
	m_defaultOStream = alOStream::get_default_ostream();
	m_defaultOStream->m_cb = this;
}

AppOStream::~AppOStream()
{
}

void AppOStream::print(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vprint(format, ap);
	va_end(ap);
}

void AppOStream::print(const wchar_t* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vprint(format, ap);
	va_end(ap);
}

void AppOStream::print(const char32_t* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vprint(format, ap);
	va_end(ap);
}

void AppOStream::vprint(const char* format, va_list arg)
{
	m_defaultOStream->vprint(format, arg);
}

void AppOStream::vprint(const wchar_t* format, va_list arg)
{
	m_defaultOStream->vprint(format, arg);
}

void AppOStream::vprint(const char32_t* format, va_list arg)
{
	m_defaultOStream->vprint(format, arg);
}

void AppOStream::on_write(const char* str)
{
	g_app->PrintLog(str);
}

void AppOStream::on_write(const wchar_t* str)
{
	g_app->PrintLog(str);
}

void AppOStream::on_write(const char32_t* str)
{
	g_app->PrintLog(str);
}


