#pragma once
#ifndef _PCSFRUST_H_
#define _PCSFRUST_H_

/*
e   end
2---3
|   |
1---0
o   origin
*/
//....

class AppSelectionFrust
{
public:
	AppSelectionFrust() {}
	~AppSelectionFrust() {}

	struct Data
	{
		alVec4 m_left[4];
		alVec4 m_top[4];
		alVec4 m_right[4];
		alVec4 m_bottom[4];
		alVec4 m_front[4];
		alVec4 m_back[4];

		// center
		alVec4 m_LC;
		alVec4 m_TC;
		alVec4 m_RC;
		alVec4 m_BC;
		alVec4 m_FrontC;
		alVec4 m_BackC;

		// normal
		alVec3 m_LN;
		alVec3 m_TN;
		alVec3 m_RN;
		alVec3 m_BN;
		alVec3 m_FrontN;
		alVec3 m_BackN;

		// only for CreateWithFrame 
		alRay m_ray1, m_ray2, m_ray3, m_ray4, m_ray5;
	};
	Data m_data;

	 void CreateWithAabb(const alAabb& aabb);
	 void CreateWithFrame(const alVec4& frame, const alVec4f& vp_rect, const alMat4& VP_invert);
	 bool PointInFrust(const alVec4& v);
	 bool LineInFrust(const alVec4& p1, const alVec4& p2);
	// using in LineInFrust
	 bool RayTest_MT(const alVec4& ray_origin, const alVec4& ray_end, const alVec4& v1, const alVec4& v2, const alVec4& v3);
};


#endif

