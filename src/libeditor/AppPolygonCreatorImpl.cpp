#include "editor.lib.h"
#include "AppMeshImpl.h"


void AppPolygonCreatorImpl::_reallocate(uint32_t size) 
{
	AppPair<AppVec3f, uint8_t>* newPositions = new AppPair<AppVec3f, uint8_t>[size];
	AppVec3f* newNormals = new AppVec3f[size];
	AppVec3f* newBinormals = new AppVec3f[size];
	AppVec3f* newTangents = new AppVec3f[size];
	AppVec2f* newtCoords = new AppVec2f[size];

	for (uint32_t i = 0; i < m_size; ++i)
	{
		newPositions[i] = m_positions[i];
		newNormals[i] = m_normals[i];
		newtCoords[i] = m_tcoords[i];
		newBinormals[i] = m_binormals[i];
		newTangents[i] = m_tangents[i];
	}

	delete[] m_positions;
	delete[] m_normals;
	delete[] m_tcoords;
	delete[] m_binormals;
	delete[] m_tangents;

	m_positions = newPositions;
	m_normals = newNormals;
	m_tcoords = newtCoords;
	m_binormals = newBinormals;
	m_tangents = newTangents;

	m_allocated = size;
}

AppPolygonCreatorImpl::AppPolygonCreatorImpl()
{
}

AppPolygonCreatorImpl::~AppPolygonCreatorImpl()
{
	if (m_positions) delete[] m_positions;
	if (m_normals) delete[] m_normals;
	if (m_tcoords) delete[] m_tcoords;
	if (m_binormals) delete[] m_binormals;
	if (m_tangents) delete[] m_tangents;
}

void AppPolygonCreatorImpl::Clear()
{
	m_size = 0; 
}

int AppPolygonCreatorImpl::Size()
{
	return m_size; 
}

void AppPolygonCreatorImpl::SetPosition(const AppVec3f& v)
{
	m_currPosition = v; 
}

void AppPolygonCreatorImpl::SetNormal(const AppVec3f& v)
{
	m_currNormal = v; 
}

void AppPolygonCreatorImpl::SetBinormal(const AppVec3f& v)
{
	m_currBinormal = v; 
}

void AppPolygonCreatorImpl::SetTangent(const AppVec3f& v)
{
	m_currTangent = v; 
}

void AppPolygonCreatorImpl::SetUV(const AppVec2f& v)
{
	m_currUV = v; 
}

void AppPolygonCreatorImpl::SetUV(float32_t u, float32_t v)
{
	m_currUV.x = u;
	m_currUV.y = v; 
}
	
void AppPolygonCreatorImpl::Add(bool weld)
{
	uint8_t flags = 0;
	if (weld)
		flags |= flag_weld;

	if (m_size == m_allocated)
	{
		_reallocate(m_allocated + (int)std::ceil(((2.f + (float)m_allocated) * 0.5f)));
	}
	m_positions[m_size] = AppPair<AppVec3f, uint8_t>(m_currPosition, flags);
	m_normals[m_size] = m_currNormal;
	m_tcoords[m_size] = m_currUV;
	m_binormals[m_size] = m_currBinormal;
	m_tangents[m_size] = m_currTangent;
	++m_size;
}

void AppPolygonCreatorImpl::ApplyMatrix(const AppMat4& W)
{
	auto positions = GetPositions();
	for (uint32_t i = 0; i < m_size; ++i)
	{
		positions[i].m_first = AppMath::Mul(positions[i].m_first, W);
	}
}

void AppPolygonCreatorImpl::Add(
	const AppVec3f& position, 
	bool weld, 
	bool selected, 
	const AppVec3f& normal, 
	const AppVec2f& tCoords) 
{
	if (m_size == m_allocated)
	{
		_reallocate(m_allocated + (int)std::ceil(((2.f + (float)m_allocated) * 0.5f)));
	}
	uint8_t flags = 0;
	if (weld)
		flags |= flag_weld;

	m_positions[m_size] = AppPair<AppVec3f, uint8_t>(position, flags);
	m_normals[m_size] = normal;
	m_tcoords[m_size] = tCoords;
	++m_size;
}

bool AppPolygonCreatorImpl::IsVisible()
{
	if (m_size > 2)
	{
		uint32_t vi = 1;
		auto vertex_1 = m_positions[0];


		uint32_t triNum = m_size - 2;

		for (uint32_t i = 0; i < triNum; ++i)
		{
			auto vertex_3 = m_positions[vi];
			auto vertex_2 = m_positions[vi + 1];

			auto a = vertex_2.m_first - vertex_1.m_first;
			auto b = vertex_3.m_first - vertex_1.m_first;

			AppVec3f n;
			a.Cross2(b, n);

			float32_t area = 0.5f * sqrt(n.Dot());

			if (area > 0.001)
				return true;

			vi += 1;
		}
	}
	return false;
}

AppPair<AppVec3f, uint8_t>* AppPolygonCreatorImpl::GetPositions() 
{
	return m_positions; 
}

AppVec3f* AppPolygonCreatorImpl::GetNormals() 
{
	return m_normals; 
}

AppVec3f* AppPolygonCreatorImpl::GetBinormals() 
{
	return m_binormals; 
}

AppVec3f* AppPolygonCreatorImpl::GetTangents() 
{
	return m_tangents; 
}

AppVec2f* AppPolygonCreatorImpl::GetTCoords() 
{
	return m_tcoords; 
}
