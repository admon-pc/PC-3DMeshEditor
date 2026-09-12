#include "editor.h"

AppPluginInterfaceImpl::AppPluginInterfaceImpl()
{
}

AppPluginInterfaceImpl::~AppPluginInterfaceImpl()
{
}

void* AppPluginInterfaceImpl::MemAlloc(size_t sz)
{
	return alMemory::Malloc(sz);
}

void AppPluginInterfaceImpl::MemFree(void* ptr)
{
	alMemory::Free(ptr);
}

bool AppPluginInterfaceImpl::GUIDIsEqual(const alGUID& g1, const alGUID& g2)
{
	return alLib::GUIDIsEqual(g1, g2);
}

size_t AppPluginInterfaceImpl::strlen(const char32_t* s)
{
	return alLib::strlen(s);
}

int32_t AppPluginInterfaceImpl::strcmp(const char32_t* p1, const char32_t* p2)
{
	return alLib::strcmp(p1, p2);
}

uint32_t AppPluginInterfaceImpl::sprintf(char32_t* str, const char32_t* format, ...)
{
	va_list args;
	va_start(args, format);

	uint32_t result = alLib::vsnprintf(str, INT_MAX, format, args);

	va_end(args);
	return result;
}

uint32_t AppPluginInterfaceImpl::snprintf(char32_t* str, size_t n, const char32_t* format, ...)
{
	va_list args;
	va_start(args, format);

	uint32_t result = alLib::vsnprintf(str, n, format, args);

	va_end(args);
	return result;
}

AppGraphicsObject* AppPluginInterfaceImpl::CreateGraphicsObject(AppGraphicsObjectDesc* desc)
{
	return 0;
}

void AppPluginInterfaceImpl::Destroy(AppGraphicsObject* go)
{
	if (go)
	{
	}
}

