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