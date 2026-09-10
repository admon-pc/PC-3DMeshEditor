#include "editor.h"

PluginInterfaceImpl::PluginInterfaceImpl()
{
}

PluginInterfaceImpl::~PluginInterfaceImpl()
{
}

void* PluginInterfaceImpl::MemAlloc(size_t sz)
{
	return alMemory::Malloc(sz);
}

void PluginInterfaceImpl::MemFree(void* ptr)
{
	alMemory::Free(ptr);
}

bool PluginInterfaceImpl::GUIDIsEqual(const alGUID& g1, const alGUID& g2)
{
	return alLib::GUIDIsEqual(g1, g2);
}

size_t PluginInterfaceImpl::strlen(const char32_t* s)
{
	return alLib::strlen(s);
}

int32_t PluginInterfaceImpl::strcmp(const char32_t* p1, const char32_t* p2)
{
	return alLib::strcmp(p1, p2);
}

uint32_t PluginInterfaceImpl::sprintf(char32_t* str, const char32_t* format, ...)
{
	va_list args;
	va_start(args, format);

	uint32_t result = alLib::vsnprintf(str, INT_MAX, format, args);

	va_end(args);
	return result;
}

uint32_t PluginInterfaceImpl::snprintf(char32_t* str, size_t n, const char32_t* format, ...)
{
	va_list args;
	va_start(args, format);

	uint32_t result = alLib::vsnprintf(str, n, format, args);

	va_end(args);
	return result;
}
