#include "editor.h"
#include "AppGraphicsObjectImpl.h"

AppGraphicsObjectImpl::AppGraphicsObjectImpl()
{
}

AppGraphicsObjectImpl::~AppGraphicsObjectImpl()
{
	AL_DESTROY(m_GPUMesh);
}


