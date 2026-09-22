#pragma once

class AppPolygonCreatorImpl : public AppPolygonCreator
{
	uint32_t m_size = 0;
	uint32_t m_allocated = 0;
	AppPair<AppVec3f, uint8_t>* m_positions = 0;
	AppVec3f* m_normals = 0;
	AppVec3f* m_binormals = 0;
	AppVec3f* m_tangents = 0;
	AppVec2f* m_tcoords = 0;

	void _reallocate(uint32_t size);

	AppVec3f m_currPosition;
	AppVec3f m_currNormal;
	AppVec3f m_currBinormal;
	AppVec3f m_currTangent;
	AppVec2f m_currUV;
public:
	AppPolygonCreatorImpl();
	virtual ~AppPolygonCreatorImpl();
	
	virtual void Clear() override;
	virtual int Size() override;

	enum
	{
		flag_weld = 0x1,
	};

	virtual void SetPosition(const AppVec3f& v) override;
	virtual void SetNormal(const AppVec3f& v) override;
	virtual void SetBinormal(const AppVec3f& v) override;
	virtual void SetTangent(const AppVec3f& v) override;
	virtual void SetUV(const AppVec2f& v) override;
	virtual void SetUV(float32_t u, float32_t v) override;
	virtual void Add(bool weld) override;
	virtual void ApplyMatrix(const AppMat4& W) override;
	virtual void Add(const AppVec3f& position, bool weld, bool selected, const AppVec3f& normal, const AppVec2f& tCoords) override;
	virtual bool IsVisible() override;
	virtual AppPair<AppVec3f, uint8_t>* GetPositions() override;
	virtual AppVec3f* GetNormals() override;
	virtual AppVec3f* GetBinormals() override;
	virtual AppVec3f* GetTangents() override;
	virtual AppVec2f* GetTCoords() override;
};
