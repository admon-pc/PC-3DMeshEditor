#pragma once
#ifndef _PCGO_H_
#define _PCGO_H_

enum class AppMeshVertexType
{
	Point, Line, Triangle
};

struct AppMeshVertexPoint
{
	AppVec3f Position;
	AppVec4f Color;
	AppVec4f Weights;
	AppVec4i Bones;
};

struct AppMeshVertexTriangle
{
	AppVec3f Position;
	AppVec2f UV1;
	AppVec2f UV2;
	AppVec3f Normal;
	AppVec3f Binormal;
	AppVec3f Tangent;
	AppVec4f Color;
	AppVec4f Weights;
	AppVec4i Bones;
};

struct AppMeshVertexLine
{
	AppVec3f Position;
	AppVec4f Color;
	AppVec4f Weights;
	AppVec4i Bones;
};

struct AppMesh
{
	AppMesh() {}
	~AppMesh() 
	{
		if (m_vertices)
			AppFree(m_vertices);
		if (m_indices)
			AppFree(m_indices);
	}

	AppMeshVertexType m_vertexType = AppMeshVertexType::Triangle;
	uint8_t* m_vertices = 0;
	uint8_t* m_indices = 0;

	AppAabb m_aabb;

	uint32_t m_vCount = 0;
	uint32_t m_iCount = 0;


	// triNum must be < 21845
	// because I use uint16_t for m_indices
	void Allocate(uint32_t triNum, AppMeshVertexType vt)
	{
		m_vertexType = vt;

		m_vCount = triNum * 3;
		m_iCount = m_vCount;

		switch (vt)
		{
		case AppMeshVertexType::Point:
			m_vertices = (uint8_t*)AppMalloc(sizeof(AppMeshVertexPoint) * m_vCount);
			break;
		case AppMeshVertexType::Line:
			m_vertices = (uint8_t*)AppMalloc(sizeof(AppMeshVertexLine) * m_vCount);
			break;
		case AppMeshVertexType::Triangle:
			m_vertices = (uint8_t*)AppMalloc(sizeof(AppMeshVertexTriangle) * m_vCount);
			break;
		}

		m_indices = (uint8_t*)AppMalloc(sizeof(uint16_t) * m_iCount);
	}

	char m_name[100];
};

// GPU mesh.
// 
class AppGraphicsObject
{
public:
	AppGraphicsObject() {}
	virtual ~AppGraphicsObject() {}

	//AppAabb m_aabb;
};

#endif

