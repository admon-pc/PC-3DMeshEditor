#include "editor.lib.h"
#include "AppMeshImpl.h"

AppVec3f AppVertexImpl::GetPosition()
{
	return m_position;
}

void AppVertexImpl::CopyData(AppVertex* other)
{
	m_position = other->GetPosition();
}

AppPolygonImpl::AppPolygonImpl()
{
}

AppPolygonImpl::~AppPolygonImpl()
{
}

AppListNode<AppPolygonImpl::_vertex_data>* AppPolygonImpl::FindVertex(AppVertex* v)
{
}

void AppPolygonImpl::CopyData(AppPolygon* other)
{
}

bool AppPolygonImpl::IsVisible()
{
}

void AppPolygonImpl::CalculateNormal()
{
}

AppVec3f AppPolygonImpl::GetFaceNormal()
{
}

AppVec3f AppPolygonImpl::GetFaceNormalCalculateNew()
{
}

void AppPolygonImpl::Flip()
{
}

void AppPolygonImpl::FixOrder(float32_t lineLineCollisionLen)
{
}


void AppPolygonMeshImpl::_add_vertex_to_list(AppVertex* newVertex)
{
}

void AppPolygonMeshImpl::_add_polygon_to_list(AppPolygon* newPolygon)
{
}

void AppPolygonMeshImpl::_remove_vertex_from_list(AppVertex* o)
{
}

void AppPolygonMeshImpl::_remove_polygon_from_list(AppPolygon* o)
{
}

void AppPolygonMeshImpl::_set_hash(AppVec3f* position)
{
}

AppPolygonMeshImpl::AppPolygonMesh()
{
}

AppPolygonMeshImpl::~AppPolygonMesh()
{
}

void AppPolygonMeshImpl::Clear()
{
}

void AppPolygonMeshImpl::AddCube(float32_t size, const AppMat4&)
{
}

void AppPolygonMeshImpl::AddSphere(uint32_t segments, float32_t radius, const AppMat4&)
{
}

void AppPolygonMeshImpl::UpdateCounts()
{
}

void AppPolygonMeshImpl::AddPolygon(AppPolygonCreator*)
{
}

AppPolygonMesh* AppPolygonMeshImpl::CreateMesh()
{
}

void AppPolygonMeshImpl::GenerateNormals(bool smooth)
{
}

void AppPolygonMeshImpl::DeletePolygon(AppPolygon*)
{
}

