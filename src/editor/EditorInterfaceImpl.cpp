#include "editor.h"

EditorInterfaceImpl::EditorInterfaceImpl()
{
}

EditorInterfaceImpl::~EditorInterfaceImpl()
{
}

void* EditorInterfaceImpl::MemAlloc(size_t sz)
{
	return alMemory::Malloc(sz);
}

void EditorInterfaceImpl::MemFree(void* ptr)
{
	alMemory::Free(ptr);
}
