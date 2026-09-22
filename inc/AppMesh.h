#pragma once
#ifndef _AppPolygonMesh_H_
#define _AppPolygonMesh_H_
#include <map>

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

class AppPolygonMesh : public AppBaseObject
{
public:
	virtual void Clear() = 0;
	virtual void AddCube(float32_t size, const AppMat4&) = 0;
	virtual void AddSphere(uint32_t segments, float32_t radius, const AppMat4&) = 0;
	virtual void UpdateCounts() = 0;
	virtual void AddPolygon(AppPolygonCreator*) = 0;
	virtual AppMesh* CreateMesh() = 0;
	virtual void GenerateNormals(bool smooth) = 0;
	virtual void DeletePolygon(AppPolygon*) = 0;
};


#endif
