#include "editor.lib.h"

#include "AppStringImpl.h"

#include <stdlib.h>

void* AppMalloc(size_t sz)
{
	return malloc(sz);
}

void* AppCalloc(size_t sz)
{
	return calloc(1, sz);
}

void AppFree(void* p)
{
	free(p);
}

FILE* AppFopenA(const char* str, const char* mode)
{
	if (str && mode)
	{
		FILE* f = 0;
		fopen_s(&f, str, mode);
		return f;
	}
	return 0;
}

FILE* AppFopenW(const wchar_t* str, const wchar_t* mode)
{
	if (str && mode)
	{
		FILE* f = 0;
		_wfopen_s(&f, str, mode);
		return f;
	}
	
	return 0;
}

AppString* AppCreateString()
{
	AppStringImpl* str = AppCreate<AppStringImpl>();
	return dynamic_cast<AppString*>(str);
}

void AppDestroyString(AppString*)
{
}
