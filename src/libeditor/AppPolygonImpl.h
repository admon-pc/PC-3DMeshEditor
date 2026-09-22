#pragma once

class AppPolygonImpl : public AppPolygon
{
public:
	AppPolygonImpl();
	virtual ~AppPolygonImpl();


	AppPolygonImpl* m_left = 0;
	AppPolygonImpl* m_right = 0;

	//miList3<alVertex*, alVec2f, alVec3f> m_verts;
	struct _vertex_data
	{
		_vertex_data() :m_vertex(0), m_flags(0) {}
		_vertex_data(AppVertexImpl* v) :m_vertex(v), m_flags(0) {}
		_vertex_data(AppVertexImpl* v, const AppVec2f& uv, const AppVec3f& normal, uint32_t flags)
			:
			m_vertex(v),
			m_uv(uv),
			m_normal(normal),
			m_flags(flags)
		{}

		bool operator==(const _vertex_data& other) {
			return m_vertex == other.m_vertex;
		}

		AppVertexImpl* m_vertex;
		AppVec2f m_uv;
		AppVec3f m_normal;
		AppVec3f m_binormal;
		AppVec3f m_tangent;
		uint32_t m_flags;
	};
	AppList<_vertex_data> m_verts;

	AppVec3f m_faceNormal;

	AppListNode<_vertex_data>* FindVertex(AppVertexImpl* v);
	virtual void CopyData(AppPolygon* other) override;
	virtual bool IsVisible() override;
	virtual void CalculateNormal() override;
	virtual AppVec3f GetFaceNormal() override;
	virtual AppVec3f GetFaceNormalCalculateNew() override;
	virtual void Flip() override;
	virtual void FixOrder(float32_t lineLineCollisionLen) override;
};
