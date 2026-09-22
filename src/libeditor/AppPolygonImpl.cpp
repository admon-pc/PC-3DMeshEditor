#include "editor.lib.h"
#include "AppMeshImpl.h"


AppPolygonImpl::AppPolygonImpl()
{
}

AppPolygonImpl::~AppPolygonImpl()
{
}

AppListNode<AppPolygonImpl::_vertex_data>* AppPolygonImpl::FindVertex(AppVertexImpl* v)
{
	auto curV = m_verts.m_head;
	auto lastV = curV->m_left;
	while (true)
	{
		if (curV->m_data.m_vertex == v)
			break;
		if (curV == lastV)
			break;
		curV = curV->m_right;
	}
	return curV;
}

void AppPolygonImpl::CopyData(AppPolygon* other)
{
}

// !!!!
// CHECK ORDER
bool AppPolygonImpl::IsVisible()
{
	// HERE
	auto vertex_1 = m_verts.m_head;
	auto vertex_3 = vertex_1->m_right;
	auto vertex_2 = vertex_3->m_right;
	while (true)
	{
		auto a = vertex_2->m_data.m_vertex->GetPosition() - vertex_1->m_data.m_vertex->GetPosition();
		auto b = vertex_3->m_data.m_vertex->GetPosition() - vertex_1->m_data.m_vertex->GetPosition();

		AppVec3f n;
		a.Cross2(b, n);

		float32_t area = 0.5f * sqrt(n.Dot());

		if (area > 0.001)
			return true;

		vertex_2 = vertex_2->m_right;
		vertex_3 = vertex_3->m_right;
		if (vertex_3 == vertex_1)
			break;
	}
	return false;
}


// !!!!
// CHECK ORDER
void AppPolygonImpl::CalculateNormal()
{
	auto n = GetFaceNormalCalculateNew();

	{
		auto vertex_1 = m_verts.m_head;
		vertex_1->m_data.m_normal = n;
		// HERE
		auto vertex_3 = vertex_1->m_right;
		auto vertex_2 = vertex_3->m_right;
		while (true)
		{
			vertex_2->m_data.m_normal = n;
			vertex_3->m_data.m_normal = n;
			// ===============================
			vertex_2 = vertex_2->m_right;
			vertex_3 = vertex_3->m_right;

			if (vertex_2 == vertex_1)
				break;
		}
	}
}
AppVec3f AppPolygonImpl::GetFaceNormal()
{
	return m_faceNormal;
}

AppVec3f AppPolygonImpl::GetFaceNormalCalculateNew()
{
	AppVec3f _n;
	auto vertex_1 = m_verts.m_head;
	auto vertex_2 = vertex_1->m_right;
	auto vertex_3 = vertex_2->m_right;

	auto e1 = vertex_2->m_data.m_vertex->GetPosition() - vertex_1->m_data.m_vertex->GetPosition();
	auto e2 = vertex_3->m_data.m_vertex->GetPosition() - vertex_1->m_data.m_vertex->GetPosition();
	auto n = e1.Cross(e2);
	n.Normalize2();

	m_faceNormal = n;
	return n;
}

void AppPolygonImpl::Flip()
{
	m_verts.reverse();
}

void AppPolygonImpl::FixOrder(float32_t lineLineCollisionLen)
{
	// only if 4 or more vertices
	uint32_t num = 0;
	auto curV = m_verts.m_head;
	auto lastV = curV->m_left;
	while (true)
	{
		++num;
		if (curV == lastV)
			break;
		curV = curV->m_right;
	}
	if (num < 4)
		return;

	for (uint32_t i = 0; i < num * 4; ++i)
	{
		curV = m_verts.m_head;
		lastV = curV->m_left;
		while (true)
		{
			AppPair<AppListNode<_vertex_data>*, AppListNode<_vertex_data>*> current;
			AppPair<AppListNode<_vertex_data>*, AppListNode<_vertex_data>*> prev;

			current.m_first = curV;
			current.m_second = current.m_first->m_right;

			prev.m_second = current.m_first;
			prev.m_first = prev.m_second->m_left;

			auto tgCurr = current.m_second->m_right;
			auto tgLast = tgCurr->m_left;
			while (true)
			{
				AppPair<AppListNode<_vertex_data>*, AppListNode<_vertex_data>*> target;
				target.m_first = tgCurr;
				target.m_second = tgCurr->m_right;

				if (target.m_first == prev.m_first)
					break;

				/*miRay r;
				r.m_origin = current.m_first->m_data.m_vertex->m_position;
				r.m_end = current.m_second->m_data.m_vertex->m_position;

				float32_t d = r.distanceToLine(target.m_first->m_data.m_vertex->m_position, target.m_second->m_data.m_vertex->m_position);
				if (d < lineLineCollisionLen)*/
				AppVec4 normal;
				AppTriangle tri1;
				AppTriangle tri2;
				{
					AppTriangle tri3;
					auto vertex = current.m_first;

					tri3.v1 = vertex->m_data.m_vertex->GetPosition();
					vertex = vertex->m_right;

					tri3.v2 = vertex->m_data.m_vertex->GetPosition();
					vertex = vertex->m_right;

					tri3.v3 = vertex->m_data.m_vertex->GetPosition();

					tri3.Update();
					tri3.e1.Cross2(tri3.e2, normal);
					normal.Normalize2();
				}

				tri1.v1 = current.m_first->m_data.m_vertex->GetPosition();
				tri1.v2 = current.m_first->m_right->m_data.m_vertex->GetPosition();
				tri1.v3 = current.m_first->m_data.m_vertex->GetPosition();

				tri2.v1 = current.m_first->m_data.m_vertex->GetPosition();
				tri2.v2 = current.m_first->m_right->m_data.m_vertex->GetPosition();
				tri2.v3 = current.m_first->m_right->m_data.m_vertex->GetPosition();

				//ip = m_origin + t * m_direction;
				tri1.v1 = tri1.v1 + 1.f * normal;
				tri1.v2 = tri1.v2 + 1.f * normal;
				tri1.v3 = tri1.v3 + 1.f * -normal;

				tri2.v1 = tri2.v1 + 1.f * -normal;
				tri2.v2 = tri2.v2 + 1.f * -normal;
				tri2.v3 = tri2.v3 + 1.f * normal;

				//{ // scale
				//	float32_t sz = 100.f;
				//	v4f center;
				//	tri.center(center);

				//	tri.v1 = ((tri.v1 - center) * sz) + center;
				//	tri.v2 = ((tri.v2 - center) * sz) + center;
				//	tri.v3 = ((tri.v3 - center) * sz) + center;
				//}
				tri1.Update();
				tri2.Update();

				AppRay r;
				r.m_origin = target.m_first->m_data.m_vertex->GetPosition();
				r.m_end = target.m_second->m_data.m_vertex->GetPosition();
				r.Update();

				float32_t T = 0.f;
				float32_t U = 0.f;
				float32_t V = 0.f;
				float32_t W = 0.f;

				bool good = 
					AppMath::RayTriangleMTf(tri1, r, true, T, U, V, W);
				//tri1.RayTest_MT(r, true, T, U, V, W);

				if (!good)
					good = AppMath::RayTriangleMTf(tri2, r, true, T, U, V, W);

				if (good)
				{
					auto tmp = target.m_first->m_data.m_vertex;
					target.m_first->m_data.m_vertex = current.m_second->m_data.m_vertex;
					current.m_second->m_data.m_vertex = tmp;
					goto end;
				}

				if (tgCurr == tgLast)
					break;

				tgCurr = tgCurr->m_right;
			}


			if (curV == lastV)
				break;
			curV = curV->m_right;
		}
	end:;
	}
}
