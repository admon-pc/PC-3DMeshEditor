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
	uint32_t m_stride = sizeof(AppMeshVertexTriangle);

	PluginAabb m_aabb;
};

struct AppGraphicsObjectDesc
{

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

