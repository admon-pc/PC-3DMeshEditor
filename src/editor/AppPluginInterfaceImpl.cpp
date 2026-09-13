#include "editor.h"

AppPluginInterfaceImpl::AppPluginInterfaceImpl(Application* app) : m_app(app)
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

AppGraphicsObject* AppPluginInterfaceImpl::CreateGraphicsObject(AppMesh* m)
{
	if (m)
	{
		alMesh mesh;
		mesh.m_iCount = m->m_iCount;
		mesh.m_vCount = m->m_vCount;
		mesh.m_stride = m->m_stride;
		switch (m->m_vertexType)
		{
		case AppMeshVertexType::Triangle:
			mesh.m_vertexType = alMeshVertexType::AnimatedTriangle;
			break;
		case AppMeshVertexType::Line:
			mesh.m_vertexType = alMeshVertexType::AnimatedLine;
			break;
		case AppMeshVertexType::Point:
			mesh.m_vertexType = alMeshVertexType::AnimatedPoint;
			break;
		}

		mesh.m_vertices = m->m_vertices;
		mesh.m_indices = m->m_indices;

		alGSMesh* gsmesh = m_app->m_gs->CreateMesh(&mesh);

		mesh.m_vertices = 0;
		mesh.m_indices = 0;
	}

	return 0;
}

void AppPluginInterfaceImpl::Destroy(AppGraphicsObject* go)
{
	if (go)
	{
	}
}

