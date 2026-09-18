#pragma once
#ifndef _APPTRIANGLE_H_
#define _APPTRIANGLE_H_

class AppTriangle
{
public:
	AppTriangle() {}
	AppTriangle(const AppVec4& _v1, const AppVec4& _v2, const AppVec4& _v3) :
		v1(_v1),
		v2(_v2),
		v3(_v3)
	{
		Update();
	}
	AppTriangle(const AppVec3f& _v1, const AppVec3& _v2, const AppVec3& _v3) :
		v1(_v1.x, _v1.y, _v1.z, 0.f),
		v2(_v2.x, _v2.y, _v2.z, 0.f),
		v3(_v3.x, _v3.y, _v3.z, 0.f)
	{
		Update();
	}

	AppVec4 v1;
	AppVec4 v2;
	AppVec4 v3;
	//AppVec4 faceNormal;
	AppVec4 normal1;
	AppVec4 normal2;
	AppVec4 normal3;
	AppVec4 e1;
	AppVec4 e2;
	AppVec4 t1;
	AppVec4 t2;
	AppVec4 t3;

	void Update() 
	{
		e1 = AppVec4(v2.x - v1.x,
			v2.y - v1.y,
			v2.z - v1.z,
			0.f);
		e2 = AppVec4(v3.x - v1.x,
			v3.y - v1.y,
			v3.z - v1.z,
			0.f);
		//	e1.cross(e2, faceNormal);
	}

	void Center(AppVec4& out)
	{
		out = (v1 + v2 + v3) * 0.3333333f;
	}

	/*bool RayTest_MT(const AppRay& ray, bool withBackFace, float32_t& T, float& U, float& V, float& W);
	bool RayTest_MT(const AppRay& ray, bool withBackFace, float64_t& T, float& U, float& V, float& W);
	bool RayTest_Watertight(const AppRay& ray, bool withBackFace, float& T, float& U, float& V, float& W);*/
};

#endif
