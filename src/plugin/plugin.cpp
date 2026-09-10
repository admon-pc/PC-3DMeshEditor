#include "plugin/plugin.h"
#include <stdlib.h>

void* PluginMalloc(size_t sz)
{
	return malloc(sz);
}

void* PluginCalloc(size_t sz)
{
	return calloc(1, sz);
}

void PluginFree(void* p)
{
	free(p);
}

FILE* PluginFopenA(const char* str, const char* mode)
{
	if (str && mode)
	{
		FILE* f = 0;
		fopen_s(&f, str, mode);
		return f;
	}
	return 0;
}

FILE* PluginFopenW(const wchar_t* str, const wchar_t* mode)
{
	if (str && mode)
	{
		FILE* f = 0;
		_wfopen_s(&f, str, mode);
		return f;
	}
	return 0;
}


PluginVec4u::PluginVec4u() 
{
	m_data[0] = 0;
	m_data[1] = 0;
	m_data[2] = 0;
	m_data[3] = 0;
}
PluginVec4u::PluginVec4u(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
	m_data[3] = w;
}

PluginVec4u::~PluginVec4u() {}
