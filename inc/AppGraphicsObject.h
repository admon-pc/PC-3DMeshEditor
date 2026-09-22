#pragma once
#ifndef _PCGO_H_
#define _PCGO_H_

class AppSceneObject;

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

enum class AppMeshIndexType : uint32_t
{
	u16,
	u32
};

class AppMesh
{
public:
	AppMesh() {}
	~AppMesh() 
	{
		if (m_vertices)
			AppFree(m_vertices);
		if (m_indices)
			AppFree(m_indices);
	}

	AppMeshVertexType m_vertexType = AppMeshVertexType::Triangle;
	AppMeshIndexType m_indexType = AppMeshIndexType::u16;
	uint8_t* m_vertices = 0;
	uint8_t* m_indices = 0;

	AppAabb m_aabb;

	uint32_t m_vCount = 0;
	uint32_t m_iCount = 0;
	uint32_t m_stride = 0;


	void Allocate(uint32_t num, AppMeshVertexType vt)
	{
		m_vertexType = vt;


		switch (vt)
		{
		case AppMeshVertexType::Point:
			m_vCount = num;
			m_iCount = m_vCount;
			m_vertices = (uint8_t*)AppMalloc(sizeof(AppMeshVertexPoint) * m_vCount);
			break;
		case AppMeshVertexType::Line:
			m_vCount = num;
			/* m_iCount
			2 points -> 0,1
			3 points -> 0,1 1,2
			4 points -> 0,1 1,2 2,3
			5 points -> 0,1 1,2 2,3 3,4
			*/
			m_iCount = (m_vCount - 1) * 2;
			m_vertices = (uint8_t*)AppMalloc(sizeof(AppMeshVertexLine) * m_vCount);
			break;
		case AppMeshVertexType::Triangle:
			m_vCount = num * 3;
			m_iCount = m_vCount;
			m_vertices = (uint8_t*)AppMalloc(sizeof(AppMeshVertexTriangle) * m_vCount);
			break;
		}

		uint32_t indexTypeSize = sizeof(uint16_t);
		if (m_iCount > 0xFFFF)
		{
			indexTypeSize = sizeof(uint32_t);
			m_indexType = AppMeshIndexType::u32;
		}

		m_indices = (uint8_t*)AppMalloc(indexTypeSize * m_iCount);
	}

	char m_name[100];
};


// GPU mesh
class AppGraphicsObject
{
public:
	AppGraphicsObject() {}
	virtual ~AppGraphicsObject() {}

	virtual void Draw(AppViewportData*, AppSceneObject*) = 0;
	//AppAabb m_aabb;
};

#endif

