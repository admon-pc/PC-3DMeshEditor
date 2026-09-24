#include "editor.lib.h"
#include "AppMeshImpl.h"
#include "AppGraphicsObject.h"

#include <vector>

void AppPolygonMeshImpl::_add_vertex_to_list(AppVertexImpl* newVertex)
{
	if (!m_first_vertex)
	{
		m_first_vertex = newVertex;
		m_first_vertex->m_right = m_first_vertex;
		m_first_vertex->m_left = m_first_vertex;
	}
	else
	{
		auto last = m_first_vertex->m_left;
		last->m_right = newVertex;
		newVertex->m_left = last;
		newVertex->m_right = m_first_vertex;
		m_first_vertex->m_left = newVertex;
	}
}

void AppPolygonMeshImpl::_add_polygon_to_list(AppPolygonImpl* newPolygon)
{
	if (!m_first_polygon)
	{
		m_first_polygon = newPolygon;
		m_first_polygon->m_right = m_first_polygon;
		m_first_polygon->m_left = m_first_polygon;
	}
	else
	{
		auto last = m_first_polygon->m_left;
		last->m_right = newPolygon;
		newPolygon->m_left = last;
		newPolygon->m_right = m_first_polygon;
		m_first_polygon->m_left = newPolygon;
	}
}

void AppPolygonMeshImpl::_remove_vertex_from_list(AppVertexImpl* o)
{
	auto l = o->m_left;
	auto r = o->m_right;
	l->m_right = r;
	r->m_left = l;

	if (o == m_first_vertex)
		m_first_vertex = o->m_right;
	if (o == m_first_vertex)
		m_first_vertex = 0;
}

void AppPolygonMeshImpl::_remove_polygon_from_list(AppPolygonImpl* o)
{
	auto l = o->m_left;
	auto r = o->m_right;
	l->m_right = r;
	r->m_left = l;

	if (o == m_first_polygon)
		m_first_polygon = o->m_right;
	if (o == m_first_polygon)
		m_first_polygon = 0;
}

void AppPolygonMeshImpl::_set_hash(AppVec3f* position)
{
	m_vertsMapHash.clear();

	char buff[2000];
	sprintf_s(buff, 2000, "%.6f%.6f%.6f", position->x, position->y, position->z);
	m_vertsMapHash = buff;
}

AppPolygonMeshImpl::AppPolygonMeshImpl()
{
}

AppPolygonMeshImpl::~AppPolygonMeshImpl()
{
	Clear();
}

void AppPolygonMeshImpl::Clear()
{
	if (m_first_polygon)
	{
		auto p = m_first_polygon;
		auto last = p->m_left;
		while (true)
		{
			auto next = p->m_right;
			p->~AppPolygonImpl();
			
			//m_allocator.Deallocate<alPolygon>(p);
			AppFree(p);

			if (p == last)
				break;
			p = next;
		}

		m_first_polygon = 0;
	}


	if (m_first_vertex)
	{
		auto v = m_first_vertex;
		auto last = v->m_left;
		while (true)
		{
			auto next = v->m_right;
			
			//v->~alVertex();
			//m_allocator.Deallocate<alVertex>(v);
			v->~AppVertexImpl();
			AppFree(v);


			if (v == last)
				break;
			v = next;
		}
		m_first_vertex = 0;
	}
}

void AppPolygonMeshImpl::AddCube(float32_t size, const AppMat4& W)
{

	float32_t pos = size;

	AppPolygonCreatorImpl pc;
	pc.Clear();
	pc.SetPosition(AppVec3f(-pos, pos, -pos));
	pc.SetUV(AppVec2f(0.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, pos, pos));
	pc.SetUV(AppVec2f(0.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, pos, pos));
	pc.SetUV(AppVec2f(1.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, pos, -pos));
	pc.SetUV(AppVec2f(1.f, 0.f));
	pc.Add(false);
	pc.ApplyMatrix(W);
	AddPolygon(&pc);

	pc.Clear();
	pc.SetPosition(AppVec3f(-pos, -pos, pos));
	pc.SetUV(AppVec2f(1.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, -pos, -pos));
	pc.SetUV(AppVec2f(1.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, -pos, -pos));
	pc.SetUV(AppVec2f(0.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, -pos, pos));
	pc.SetUV(AppVec2f(0.f, 1.f));
	pc.Add(false);
	pc.ApplyMatrix(W);
	AddPolygon(&pc);

	pc.Clear();
	pc.SetPosition(AppVec3f(pos, -pos, pos));
	pc.SetUV(AppVec2f(1.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, pos, pos));
	pc.SetUV(AppVec2f(1.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, pos, pos));
	pc.SetUV(AppVec2f(0.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, -pos, pos));
	pc.SetUV(AppVec2f(0.f, 1.f));
	pc.Add(false);
	pc.ApplyMatrix(W);
	AddPolygon(&pc);

	pc.Clear();
	pc.SetPosition(AppVec3f(pos, pos, -pos));
	pc.SetUV(AppVec2f(0.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, -pos, -pos));
	pc.SetUV(AppVec2f(0.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, -pos, -pos));
	pc.SetUV(AppVec2f(1.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, pos, -pos));
	pc.SetUV(AppVec2f(1.f, 0.f));
	pc.Add(false);
	pc.ApplyMatrix(W);
	AddPolygon(&pc);

	pc.Clear();
	pc.SetPosition(AppVec3f(pos, pos, pos));
	pc.SetUV(AppVec2f(0.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, -pos, pos));
	pc.SetUV(AppVec2f(0.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, -pos, -pos));
	pc.SetUV(AppVec2f(1.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(pos, pos, -pos));
	pc.SetUV(AppVec2f(1.f, 0.f));
	pc.Add(false);
	pc.ApplyMatrix(W);
	AddPolygon(&pc);

	pc.Clear();
	pc.SetPosition(AppVec3f(-pos, -pos, pos));
	pc.SetUV(AppVec2f(1.f, 1.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, pos, pos));
	pc.SetUV(AppVec2f(1.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, pos, -pos));
	pc.SetUV(AppVec2f(0.f, 0.f));
	pc.Add(false);
	pc.SetPosition(AppVec3f(-pos, -pos, -pos));
	pc.SetUV(AppVec2f(0.f, 1.f));
	pc.Add(false);
	pc.ApplyMatrix(W);
	AddPolygon(&pc);
}

void AppPolygonMeshImpl::AddSphere(
	uint32_t _segments,
	float32_t radius,
	const AppMat4& W)
{
	AppPolygonCreatorImpl pc;

	if (_segments < 3) _segments = 3;
	if (_segments > 32) _segments = 32;

	uint32_t segments = _segments;
	uint32_t segmentsXi = segments - 1;

	uint32_t lastSegment = segments - 1;

	float32_t angleStepY = 360.f / float32_t(segments);
	float32_t angleStepX = 180.f / float32_t(segments - 1);

	float32_t angleY = 0.f;
	float32_t angleX = 0.f;
	//printf("angleStepY [%f]\n", angleStepY);
	//printf("angleStepX [%f]\n", angleStepX);

	AppVec3f p3prev;
	AppVec3f p4prev;

	AppVec3f p_base(0.f, radius, 0.f);

	float32_t uvstep = 1.f / float32_t(segments);
	float32_t currU = 1.f;
	float32_t currV = 0;
	bool weld = true;

	for (uint32_t yi = 0; yi < segments; ++yi)
	{
		for (uint32_t xi = 0; xi < segmentsXi; ++xi)
		{
			AppMat4 mX(AppQuaternion(AppMath::DegToRad(angleX), 0.f, 0.f));
			AppVec3f p1_base = AppMath::Mul(p_base, mX);
			AppMat4 mXn(AppQuaternion(AppMath::DegToRad(angleX + angleStepX), 0.f, 0.f));
			AppVec3f p2_base = AppMath::Mul(p_base, mXn);

			if (xi == 0)
			{
				// in this branch I can create pyramids and cones				
				AppMat4 mY(AppQuaternion(0.f, AppMath::DegToRad(angleY), 0.f));
				AppMat4 mYn(AppQuaternion(0.f, AppMath::DegToRad(angleY + angleStepY), 0.f));

				AppVec3f p1 = p1_base;
				AppVec3f p2 = AppMath::Mul(p2_base, mY);
				AppVec3f p3 = AppMath::Mul(p2_base, mYn);

				pc.Clear();
				pc.SetPosition(p1);
				pc.SetUV(0.5f, 0.f);
				pc.Add(weld);
				pc.SetPosition(p3);
				pc.SetUV(currU - uvstep, currV + uvstep);
				pc.Add(weld);
				pc.SetPosition(p2);
				pc.SetUV(currU, currV + uvstep);
				pc.Add(weld);
				pc.ApplyMatrix(W);
				AddPolygon(&pc);
			}
			else if (xi == segmentsXi - 1)
			{
				AppMat4 mY(AppQuaternion(0.f, AppMath::DegToRad(angleY), 0.f));
				AppMat4 mYn(AppQuaternion(0.f, AppMath::DegToRad(angleY + angleStepY), 0.f));

				AppVec3f p1 = AppMath::Mul(p1_base, mY);
				AppVec3f p2 = p2_base;
				AppVec3f p3 = AppMath::Mul(p1_base, mYn);


				// need to fix UV in this part

				pc.Clear();
				pc.SetPosition(p1);
				pc.SetUV(currU, currV);
				pc.Add(weld);
				pc.SetPosition(p3);
				pc.SetUV(currU - uvstep, currV);
				pc.Add(weld);
				pc.SetPosition(p2);
				pc.SetUV(0.5f, 1.f);
				pc.Add(weld);
				pc.ApplyMatrix(W);
				AddPolygon(&pc);
			}
			else
			{
				AppMat4 mY(AppQuaternion(0.f, AppMath::DegToRad(angleY), 0.f));
				AppMat4 mYn(AppQuaternion(0.f, AppMath::DegToRad(angleY + angleStepY), 0.f));

				AppVec3f p1 = AppMath::Mul(p1_base, mY);
				AppVec3f p2 = AppMath::Mul(p2_base, mY);
				AppVec3f p3 = AppMath::Mul(p2_base, mYn);
				AppVec3f p4 = AppMath::Mul(p1_base, mYn);
				/*if (yi > 0)
					p2 = p3prev;
				else
					p2 = AppMath::Mul(p1, mX);
				p3prev = p3;*/

				pc.Clear();
				pc.SetPosition(p2);
				pc.SetUV(currU, currV + uvstep);
				pc.Add(weld);
				pc.SetPosition(p1);
				pc.SetUV(currU, currV);
				pc.Add(weld);
				pc.SetPosition(p4);
				pc.SetUV(currU - uvstep, currV);
				pc.Add(weld);
				pc.SetPosition(p3);
				pc.SetUV(currU - uvstep, currV + uvstep);
				pc.Add(weld);
				pc.ApplyMatrix(W);
				AddPolygon(&pc);
			}

			angleX += angleStepX;
			currV += uvstep;
		}

		currV = 0.f;
		currU -= uvstep;

		angleX = 0.f;
		angleY += angleStepY;
	}

	// delete this later
	//alArray<alPolygon*> invisiblePolygons;
	//auto c = m_first_polygon;
	//if (c)
	//{
	//	auto l = c->m_left;
	//	while (true)
	//	{
	//		if (!c->IsVisible())
	//		{
	//			printf("A\n");
	//			invisiblePolygons.push_back(c);
	//		}
	//		if (c == l)
	//			break;
	//		c = c->m_right;
	//	}
	//}
	//if (invisiblePolygons.size())
	//{
	//	for (size_t i = 0; i < invisiblePolygons.m_size; ++i)
	//	{
	//		//DeletePolygon(invisiblePolygons.m_data[i]);
	//	}
	//}
}

void AppPolygonMeshImpl::UpdateCounts()
{
	m_vertexCount = 0;
	m_polygonCount = 0;
	{
		auto c = m_first_vertex;
		if (c)
		{
			auto l = c->m_left;
			while (true)
			{
				++m_vertexCount;
				if (c == l)
					break;
				c = c->m_right;
			}
		}
	}
	{
		auto c = m_first_polygon;
		if (c)
		{
			auto l = c->m_left;
			while (true)
			{
				++m_polygonCount;
				auto cv = c->m_verts.m_head;
				auto lv = cv->m_left;
				while (true)
				{
					//++m_uvCount;
					if (cv == lv)
						break;
					cv = cv->m_right;
				}
				if (c == l)
					break;
				c = c->m_right;
			}
		}
	}
}

void AppPolygonMeshImpl::AddPolygon(AppPolygonCreator* pc)
{
	if (!pc->IsVisible())
	{
		//	return;
	}

	int polygonVertexCount = pc->Size();
	if (polygonVertexCount > 2)
	{
		AppPolygonImpl* newPolygon = AppCreate<AppPolygonImpl>();
		if (!m_first_polygon)
		{
			m_first_polygon = newPolygon;
			m_first_polygon->m_right = m_first_polygon;
			m_first_polygon->m_left = m_first_polygon;
		}
		else
		{
			auto last = m_first_polygon->m_left;
			last->m_right = newPolygon;
			newPolygon->m_left = last;
			newPolygon->m_right = m_first_polygon;
			m_first_polygon->m_left = newPolygon;
		}

		auto positions = pc->GetPositions();
		auto normals = pc->GetNormals();
		auto tCoords = pc->GetTCoords();
		auto binormals = pc->GetBinormals();
		auto tangents = pc->GetTangents();

		/*	alPolygon testPolygon;
			for (int i = 0; i < polygonVertexCount; ++i)
			{
				alPolygon::_vertex_data newVD;
				newVD.m_vertex = newVertex;
				testPolygon.m_verts.push_back();
			}*/

		for (int i = 0; i < polygonVertexCount; ++i)
		{
			AppVertexImpl* newVertex = 0;
			m_aabb.Add(positions[i].m_first);

			if (positions[i].m_second & AppPolygonCreatorImpl::flag_weld)
			{
				_set_hash(&positions[i].m_first);

				auto find_result = m_weldMap.find(m_vertsMapHash);
				if (find_result == m_weldMap.end())
				{
					//newVertex = m_allocatorVertex->Allocate();
					//new(newVertex)miVertex();
					newVertex = AppCreate<AppVertexImpl>();

					newVertex->m_position = positions[i].m_first;

					m_weldMap[m_vertsMapHash] = newVertex;
					_add_vertex_to_list(newVertex);
				}
				else
				{
					newVertex = find_result->second;
				}
			}
			else
			{
				//newVertex = m_allocatorVertex->Allocate();
				//new(newVertex)miVertex();
				newVertex = AppCreate<AppVertexImpl>();

				newVertex->m_position = positions[i].m_first;

				_add_vertex_to_list(newVertex);
			}

			// add newPolygon to vertex and newVertex to polygon
			// only once
			if (newVertex->m_polygons.find(newPolygon) == 0)
			{
				newVertex->m_polygons.push_back(newPolygon);
				AppPolygonImpl::_vertex_data newVD;
				newVD.m_vertex = newVertex;
				newVD.m_uv = tCoords[i];
				newVD.m_normal = normals[i];
				newVD.m_binormal = binormals[i];
				newVD.m_tangent = tangents[i];

				newPolygon->m_verts.push_back(newVD);
			}
		}

		//		newPolygon->CalculateNormal();
	}
}

AppMesh* AppPolygonMeshImpl::CreateMesh(AppMeshVertexType vt, AppArray<AppMesh*>* arr, uint32_t polygonLimit)
{
	AppMesh* newMesh = 0;
	if (!polygonLimit)
		polygonLimit = 0xFFFFFFFF;

	if (m_first_polygon)
	{
		uint32_t polygonCounter = 0;
		uint32_t triCounter = 0;

		std::vector<AppPolygonImpl*> polygonsForMesh;

		auto cp = m_first_polygon;
		auto lp = cp->m_left;
		while (true)
		{
			polygonsForMesh.push_back(cp);
			++polygonCounter;

			uint32_t numVertPerPolygon = 0;
			auto cv = cp->m_verts.m_head;
			auto lv = cv->m_left;
			while (true)
			{
				//++m_uvCount;
				++numVertPerPolygon;

				if (cv == lv)
					break;
				cv = cv->m_right;
			}

			uint32_t numOfTris = numVertPerPolygon - 2;
			triCounter += numOfTris;

			if ((polygonCounter == polygonLimit)
				|| (cp == lp))
			{
				newMesh = AppCreate<AppMesh>();
				newMesh->Allocate(triCounter, vt);
				uint32_t indexCounter = 0;
				uint32_t* inds32 = 0;
				uint16_t* inds16 = 0;
				inds32 = (uint32_t*)newMesh->m_indices;
				inds16 = (uint16_t*)newMesh->m_indices;

				for (uint32_t i = 0; i < newMesh->m_iCount; ++i)
				{
					if (newMesh->m_indexType == AppMeshIndexType::u32)
					{
						*inds32 = indexCounter;
						++inds32;
						++indexCounter;
					}
					else
					{
						*inds16 = indexCounter;
						++inds16;
						++indexCounter;
					}
				}
				AppMeshVertexTriangle* meshVerts = (AppMeshVertexTriangle*)newMesh->m_vertices;
				for (size_t oi = 0; oi < polygonsForMesh.size(); ++oi)
				{
					auto polygon = polygonsForMesh.data()[oi];
					auto vertex_1 = polygon->m_verts.m_head;
					auto vertex_3 = vertex_1->m_right;
					auto vertex_2 = vertex_3->m_right;
					while (true)
					{
						meshVerts->Position = vertex_1->m_data.m_vertex->m_position;
						meshVerts->UV1 = vertex_1->m_data.m_uv;
						meshVerts->Normal = vertex_1->m_data.m_normal;
						meshVerts->Binormal = vertex_1->m_data.m_binormal;
						meshVerts->Tangent = vertex_1->m_data.m_tangent;
						++meshVerts;

						meshVerts->Position = vertex_2->m_data.m_vertex->m_position;
						meshVerts->UV1 = vertex_2->m_data.m_uv;
						meshVerts->Normal = vertex_2->m_data.m_normal;
						meshVerts->Binormal = vertex_2->m_data.m_binormal;
						meshVerts->Tangent = vertex_2->m_data.m_tangent;
						++meshVerts;

						meshVerts->Position = vertex_3->m_data.m_vertex->m_position;
						meshVerts->UV1 = vertex_3->m_data.m_uv;
						meshVerts->Normal = vertex_3->m_data.m_normal;
						meshVerts->Binormal = vertex_3->m_data.m_binormal;
						meshVerts->Tangent = vertex_3->m_data.m_tangent;
						++meshVerts;

						vertex_2 = vertex_2->m_right;
						vertex_3 = vertex_3->m_right;

						if (vertex_2 == vertex_1)
							break;
					}
				}

				arr->push_back(newMesh);
				/////////////////////////////////////////////////////////////
				polygonsForMesh.clear();
				polygonCounter = 0;
				triCounter = 0;
			}

			if (cp == lp)
				break;
			cp = cp->m_right;
		}
	}

	//printf("numOfVerts: %u\n", numOfVerts);

	return arr ? 0 : newMesh;
}

void AppPolygonMeshImpl::GenerateNormals(bool smooth)
{
	auto current_polygon = m_first_polygon;
	auto last_polygon = current_polygon->m_left;
	while (true)
	{
		current_polygon->CalculateNormal();
		if (current_polygon == last_polygon)
			break;
		current_polygon = current_polygon->m_right;
	}

	if (smooth)
	{
		auto curr = m_first_vertex;
		auto last = curr->m_left;
		while (true)
		{
			uint32_t nn = 0;
			AppVec3f normal;
			{
				auto cp = curr->m_polygons.m_head;
				auto lp = cp->m_left;
				while (true)
				{

					auto vNode = cp->m_data->FindVertex(curr);
					if (vNode)
					{
						normal += cp->m_data->GetFaceNormal();
						//normal += vNode->m_data.m_normal;
						++nn;
					}

					if (cp == lp)
						break;
					cp = cp->m_right;
				}
			}

			//	printf(" %u", nn);
			if (nn)
				normal = normal / (float32_t)nn;
			normal.Normalize2();

			////	auto node = curr->m_polygons.m_head->m_data->FindVertex(curr);
			////	node->m_data.m_normal = normal;

				//normal.Normalize2();
			{
				auto cp = curr->m_polygons.m_head;
				auto lp = cp->m_left;
				while (true)
				{
					auto vNode = cp->m_data->FindVertex(curr);
					if (vNode)
						vNode->m_data.m_normal = normal;
					if (cp == lp)
						break;
					cp = cp->m_right;
				}
			}

			if (curr == last)
				break;
			curr = curr->m_right;
		}
	}
}

void AppPolygonMeshImpl::DeletePolygon(AppPolygon*)
{
}

AppAabb* AppPolygonMeshImpl::GetAABB()
{
	return &m_aabb;
}

void AppPolygonMeshImpl::SetPolygonFlag(uint32_t f)
{
	auto current_polygon = m_first_polygon;
	auto last_polygon = current_polygon->m_left;
	while (true)
	{
		current_polygon->m_flags |= f;
		if (current_polygon == last_polygon)
			break;
		current_polygon = current_polygon->m_right;
	}
}

void AppPolygonMeshImpl::RemovePolygonFlag(uint32_t f)
{
	auto current_polygon = m_first_polygon;
	auto last_polygon = current_polygon->m_left;
	while (true)
	{
		current_polygon->m_flags &= ~f;

		if (current_polygon == last_polygon)
			break;
		current_polygon = current_polygon->m_right;
	}
}

