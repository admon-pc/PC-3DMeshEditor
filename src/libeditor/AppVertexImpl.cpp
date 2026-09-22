#include "editor.lib.h"
#include "AppMeshImpl.h"

AppVertexImpl::AppVertexImpl() {}
AppVertexImpl::~AppVertexImpl() {}

AppVec3f AppVertexImpl::GetPosition()
{
	return m_position;
}

void AppVertexImpl::CopyData(AppVertex* other)
{
	m_position = other->GetPosition();
}
