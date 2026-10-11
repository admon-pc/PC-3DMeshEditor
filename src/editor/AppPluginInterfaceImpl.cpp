#include "editor.h"
#include "AppGraphicsObjectImpl.h"

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
	AppGraphicsObjectImpl* result = 0;
	if (m)
	{
		alMesh mesh;
		mesh.m_iCount = m->m_iCount;
		mesh.m_vCount = m->m_vCount;
		switch (m->m_vertexType)
		{
		case AppMeshVertexType::Triangle:
			mesh.m_vertexType = alMeshVertexType::AnimatedTriangle;
			mesh.m_stride = sizeof(alVertexAnimatedTriangle);
			break;
		case AppMeshVertexType::Line:
			mesh.m_vertexType = alMeshVertexType::AnimatedLine;
			mesh.m_stride = sizeof(alVertexAnimatedLine);
			break;
		case AppMeshVertexType::Point:
			mesh.m_vertexType = alMeshVertexType::AnimatedPoint;
			mesh.m_stride = sizeof(alVertexAnimatedPoint);
			break;
		}

		switch (m->m_indexType)
		{
		default:
		case AppMeshIndexType::u16:
			mesh.m_indexType = alMeshIndexType::u16;
			break;
		case AppMeshIndexType::u32:
			mesh.m_indexType = alMeshIndexType::u32;
			break;
		}

		mesh.m_vertices = m->m_vertices;
		mesh.m_indices = m->m_indices;

		alGSMesh* gsmesh = m_app->m_gs->CreateMesh(&mesh);
		if (gsmesh)
		{
			result = AppCreate<AppGraphicsObjectImpl>(m_app->m_gs, mesh.m_vertexType);
			result->m_GPUMesh = gsmesh;
		//	result->m_aabb = m->m_aabb;
		}

		mesh.m_vertices = 0;
		mesh.m_indices = 0;
	}

	return result;
}

AppScene* AppPluginInterfaceImpl::GetScene()
{
	return m_app->m_scene;
}

void AppPluginInterfaceImpl::GetRayFromScreen(
	AppRay* ray, 
	const AppVec2f& coords,
	const AppVec4f& viewportRect, 
	const AppMat4& VPInvert)
{
	AppVec2f point;
	point.x = coords.x - viewportRect.x;
	point.y = coords.y - viewportRect.y;
	float pt_x = ((float)point.x / (viewportRect.z - viewportRect.x)) * 2.f - 1.f;
	float pt_y = -((float)point.y / (viewportRect.w - viewportRect.y)) * 2.f + 1.f;

	AppMat4 VPI;
	auto VPIptr = VPI.GetPtr();
	auto VPInvertptr = VPInvert.GetPtrConst();
	for (int i = 0; i < 16; ++i) {
		VPIptr[i] = VPInvertptr[i];
	}
	auto O = AppMath::Mul(AppVec4f(pt_x, pt_y, m_app->m_gpuDepthRange.x, 1.f), VPI);
	auto E = AppMath::Mul(AppVec4f(pt_x, pt_y, m_app->m_gpuDepthRange.y, 1.f), VPI);

	ray->m_origin.Set(O.x, O.y, O.z, 1.f);
	ray->m_end.Set(E.x, E.y, E.z, 1.f);

	ray->m_origin.w = 1.0f / ray->m_origin.w;
	ray->m_origin.x *= ray->m_origin.w;
	ray->m_origin.y *= ray->m_origin.w;
	ray->m_origin.z *= ray->m_origin.w;

	ray->m_end.w = 1.0f / ray->m_end.w;
	ray->m_end.x *= ray->m_end.w;
	ray->m_end.y *= ray->m_end.w;
	ray->m_end.z *= ray->m_end.w;

	ray->Update();
}

