#pragma once
#ifndef _PCGO_H_
#define _PCGO_H_

enum class AppMeshVertexType
{
	Point, Line, Triangle
};

struct AppMeshVertexPoint
{
	plVec3f Position;
	plVec4f Color;
	plVec4f Weights;
	plVec4i Bones;
};

struct AppMeshVertexTriangle
{
	plVec3f Position;
	plVec2f UV1;
	plVec2f UV2;
	plVec3f Normal;
	plVec3f Binormal;
	plVec3f Tangent;
	plVec4f Color;
	plVec4f Weights;
	plVec4i Bones;
};

struct AppMeshVertexLine
{
	plVec3f Position;
	plVec4f Color;
	plVec4f Weights;
	plVec4i Bones;
};

struct AppMesh
{
	AppMeshVertexType m_vertexType = AppMeshVertexType::Triangle;
	uint8_t* m_vertices = 0;
	uint8_t* m_indices = 0;

	PluginAabb m_aabb;

	uint32_t m_vCount = 0;
	uint32_t m_iCount = 0;
	uint32_t m_stride = 0;

	char m_name[100];
};

// GPU mesh.
// 
class AppGraphicsObject
{
public:
	AppGraphicsObject() {}
	virtual ~AppGraphicsObject() {}


};

#endif

