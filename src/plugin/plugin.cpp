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


