#pragma once

class AppPolygonMeshImpl : public AppPolygonMesh
{
	void _add_vertex_to_list(AppVertexImpl* newVertex);
	void _add_polygon_to_list(AppPolygonImpl* newPolygon);
	void _remove_vertex_from_list(AppVertexImpl* o);
	void _remove_polygon_from_list(AppPolygonImpl* o);
	void _set_hash(AppVec3f* position);

	std::map<std::string, AppVertexImpl*> m_weldMap;
	std::string m_vertsMapHash;
public:
	AppPolygonMeshImpl();
	virtual ~AppPolygonMeshImpl();

	void Clear();
	void AddCube(float32_t size, const AppMat4&);
	void AddSphere(uint32_t segments, float32_t radius, const AppMat4&);

	AppPolygonImpl* m_first_polygon = 0;
	AppVertexImpl* m_first_vertex = 0;
	//miSkeleton* m_skeleton = 0;

	uint32_t m_vertexCount = 0;
	uint32_t m_polygonCount = 0;

	AppAabb m_aabb;

	void UpdateCounts();

	void AddPolygon(AppPolygonCreator*);
	AppMesh* CreateMesh();

	void GenerateNormals(bool smooth);
	void DeletePolygon(AppPolygon*);
};
