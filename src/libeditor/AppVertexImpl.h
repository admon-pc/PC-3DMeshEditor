#pragma once

class AppPolygonImpl;
class AppVertexImpl : public AppVertex
{
public:
	AppVertexImpl();
	virtual ~AppVertexImpl();

	AppVertexImpl* m_left = 0;
	AppVertexImpl* m_right = 0;
	AppList<AppPolygonImpl*> m_polygons;


	AppVec3f m_position;

	virtual void CopyData(AppVertex* other) override;
	virtual AppVec3f GetPosition() override;

};

