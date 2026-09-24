#pragma once
#ifndef _AppPolygonMesh_H_
#define _AppPolygonMesh_H_
#include <map>

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

	// num is triNum for AppMeshVertexType::Triangle
	// for other type is number of points
	void Allocate(uint32_t num, AppMeshVertexType vt)
	{
		m_vertexType = vt;


		switch (vt)
		{
		case AppMeshVertexType::Point:
			m_vCount = num;
			m_iCount = m_vCount;
			m_vertices = (uint8_t*)AppMalloc(sizeof(AppMeshVertexPoint) * m_vCount);
			m_stride = sizeof(AppMeshVertexPoint);
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
			m_stride = sizeof(AppMeshVertexLine);
			break;
		case AppMeshVertexType::Triangle:
			m_vCount = num * 3;
			m_iCount = m_vCount;
			m_vertices = (uint8_t*)AppMalloc(sizeof(AppMeshVertexTriangle) * m_vCount);
			m_stride = sizeof(AppMeshVertexTriangle);
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


class AppPolygon;
class AppVertex : public AppBaseObject
{
public:
	virtual void CopyData(AppVertex* other) = 0;
	virtual AppVec3f GetPosition() = 0;
};

class AppPolygon : public AppBaseObject
{
public:
	virtual void CopyData(AppPolygon* other) = 0;
	//virtual AppListNode<_vertex_data>* FindVertex(AppVertex* v);
	virtual bool IsVisible() = 0;
	virtual void CalculateNormal() = 0;
	virtual AppVec3f GetFaceNormal() = 0;
	virtual AppVec3f GetFaceNormalCalculateNew() = 0;
	virtual void Flip() = 0;
	virtual void FixOrder(float32_t lineLineCollisionLen) = 0;

};


// use this when you need to create a new model
// don't forget to call Clear() before or after every polygon
class AppPolygonCreator : public AppBaseObject
{
public:
	virtual void Clear() = 0;
	virtual int Size() = 0;
	virtual void SetPosition(const AppVec3f& v) = 0;
	virtual void SetNormal(const AppVec3f& v) = 0;
	virtual void SetBinormal(const AppVec3f& v) = 0;
	virtual void SetTangent(const AppVec3f& v) = 0;
	virtual void SetUV(const AppVec2f& v) = 0;
	virtual void SetUV(float32_t u, float32_t v) = 0;
	virtual void Add(bool weld) = 0;
	virtual void ApplyMatrix(const AppMat4& W) = 0;
	virtual void Add(const AppVec3f& position, bool weld, bool selected, const AppVec3f& normal, const AppVec2f& tCoords) = 0;
	virtual bool IsVisible() = 0;
	virtual AppPair<AppVec3f, uint8_t>* GetPositions() = 0;
	virtual AppVec3f* GetNormals() = 0;
	virtual AppVec3f* GetBinormals() = 0;
	virtual AppVec3f* GetTangents() = 0;
	virtual AppVec2f* GetTCoords() = 0;
};

class AppMesh; // AppGraphicsObject.h
class AppAabb; // AppAabb.h

class AppPolygonMesh : public AppBaseObject
{
public:
	virtual void Clear() = 0;
	virtual void AddCube(float32_t size, const AppMat4&) = 0;
	virtual void AddSphere(uint32_t segments, float32_t radius, const AppMat4&) = 0;
	virtual void UpdateCounts() = 0;
	virtual void AddPolygon(AppPolygonCreator*) = 0;

	// This method will create mesh buffer that can be used for creating GPU mesh buffer.
	// `arr` is optional. if you set `arr`, return will be ignored (0 will return),
	//   and result will be placed into `arr`. triLimit is `triangle limit`, if
	//   this AppPolygonMesh has many triangles, it will be devided using this limit,
	//   all parts will be stored in `arr`. if triLimit is 0 then this will be NO limit.
	//   But the idea of `arr` is for deviding the model on many parts.
	virtual AppMesh* CreateMesh(AppMeshVertexType, AppArray<AppMesh*>* arr, uint32_t triLimit) = 0;

	virtual void GenerateNormals(bool smooth) = 0;
	virtual void DeletePolygon(AppPolygon*) = 0;
	virtual AppAabb* GetAABB() = 0;

	virtual void SetPolygonFlag(uint32_t) = 0;
	virtual void RemovePolygonFlag(uint32_t) = 0;
};


#endif
