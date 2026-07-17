#include "editor.h"
extern Application* g_app;

void AppSelectionFrust::CreateWithAabb(const alAabb& aabb)
{
	m_data.m_top[0].Set(aabb.m_min.x, aabb.m_max.y, aabb.m_min.z, 1.f);
	m_data.m_top[1].Set(aabb.m_max.x, aabb.m_max.y, aabb.m_min.z, 1.f);
	m_data.m_top[2].Set(aabb.m_min.x, aabb.m_max.y, aabb.m_max.z, 1.f);
	m_data.m_top[3].Set(aabb.m_max.x, aabb.m_max.y, aabb.m_max.z, 1.f);

	m_data.m_right[0].Set(aabb.m_max.x, aabb.m_max.y, aabb.m_min.z, 1.f);
	m_data.m_right[1].Set(aabb.m_max.x, aabb.m_min.y, aabb.m_min.z, 1.f);
	m_data.m_right[2].Set(aabb.m_max.x, aabb.m_max.y, aabb.m_max.z, 1.f);
	m_data.m_right[3].Set(aabb.m_max.x, aabb.m_min.y, aabb.m_max.z, 1.f);

	m_data.m_bottom[0].Set(aabb.m_max.x, aabb.m_min.y, aabb.m_min.z, 1.f);
	m_data.m_bottom[1].Set(aabb.m_min.x, aabb.m_min.y, aabb.m_min.z, 1.f);
	m_data.m_bottom[2].Set(aabb.m_max.x, aabb.m_min.y, aabb.m_max.z, 1.f);
	m_data.m_bottom[3].Set(aabb.m_min.x, aabb.m_min.y, aabb.m_max.z, 1.f);

	m_data.m_left[0].Set(aabb.m_min.x, aabb.m_min.y, aabb.m_min.z, 1.f);
	m_data.m_left[1].Set(aabb.m_min.x, aabb.m_max.y, aabb.m_min.z, 1.f);
	m_data.m_left[2].Set(aabb.m_min.x, aabb.m_min.y, aabb.m_max.z, 1.f);
	m_data.m_left[3].Set(aabb.m_min.x, aabb.m_max.y, aabb.m_max.z, 1.f);

	m_data.m_front[0].Set(aabb.m_min.x, aabb.m_min.y, aabb.m_max.z, 1.f);
	m_data.m_front[1].Set(aabb.m_min.x, aabb.m_max.y, aabb.m_max.z, 1.f);
	m_data.m_front[2].Set(aabb.m_max.x, aabb.m_min.y, aabb.m_max.z, 1.f);
	m_data.m_front[3].Set(aabb.m_max.x, aabb.m_max.y, aabb.m_max.z, 1.f);

	m_data.m_back[0].Set(aabb.m_min.x, aabb.m_min.y, aabb.m_min.z, 1.f);
	m_data.m_back[1].Set(aabb.m_min.x, aabb.m_max.y, aabb.m_min.z, 1.f);
	m_data.m_back[2].Set(aabb.m_max.x, aabb.m_min.y, aabb.m_min.z, 1.f);
	m_data.m_back[3].Set(aabb.m_max.x, aabb.m_max.y, aabb.m_min.z, 1.f);

	alVec3 e1, e2;

	e1 = m_data.m_right[1] - m_data.m_right[0];
	e2 = m_data.m_right[2] - m_data.m_right[0];
	e1.Cross2(e2, m_data.m_RN);
	m_data.m_RC = m_data.m_right[0] + m_data.m_right[1] + m_data.m_right[2] + m_data.m_right[3];
	m_data.m_RC *= 0.25;

	e1 = m_data.m_bottom[1] - m_data.m_bottom[0];
	e2 = m_data.m_bottom[2] - m_data.m_bottom[0];
	e1.Cross2(e2, m_data.m_BN);
	m_data.m_BC = m_data.m_bottom[0] + m_data.m_bottom[1] + m_data.m_bottom[2] + m_data.m_bottom[3];
	m_data.m_BC *= 0.25;

	e1 = m_data.m_top[1] - m_data.m_top[0];
	e2 = m_data.m_top[2] - m_data.m_top[0];
	e1.Cross2(e2, m_data.m_TN);
	m_data.m_TC = m_data.m_top[0] + m_data.m_top[1] + m_data.m_top[2] + m_data.m_top[3];
	m_data.m_TC *= 0.25;

	e1 = m_data.m_left[1] - m_data.m_left[0];
	e2 = m_data.m_left[2] - m_data.m_left[0];
	e1.Cross2(e2, m_data.m_LN);
	m_data.m_LC = m_data.m_left[0] + m_data.m_left[1] + m_data.m_left[2] + m_data.m_left[3];
	m_data.m_LC *= 0.25;

	e1 = m_data.m_front[1] - m_data.m_front[0];
	e2 = m_data.m_front[2] - m_data.m_front[0];
	e1.Cross2(e2, m_data.m_FrontN);
	m_data.m_FrontC = m_data.m_front[0] + m_data.m_front[1] + m_data.m_front[2] + m_data.m_front[3];
	m_data.m_FrontC *= 0.25;

	e1 = m_data.m_back[1] - m_data.m_back[0];
	e2 = m_data.m_back[2] - m_data.m_back[0];
	e1.Cross2(e2, m_data.m_BackN);
	m_data.m_BackC = m_data.m_back[0] + m_data.m_back[1] + m_data.m_back[2] + m_data.m_back[3];
	m_data.m_BackC *= 0.25;

	m_data.m_RN.Normalize2();
	m_data.m_LN.Normalize2();
	m_data.m_TN.Normalize2();
	m_data.m_BN.Normalize2();
	m_data.m_BackN.Normalize2();
	m_data.m_FrontN.Normalize2();
}

void AppSelectionFrust::CreateWithFrame(const alVec4& frame, const alVec4& vp_rect, const alMat4& VP_invert)
{
	// get 4 rays from screen


	g_app->GetRayFromScreen(&m_data.m_ray1, alVec2f(frame.x, frame.y), vp_rect, VP_invert);
	g_app->GetRayFromScreen(&m_data.m_ray2, alVec2f(frame.z, frame.y), vp_rect, VP_invert);
	g_app->GetRayFromScreen(&m_data.m_ray3, alVec2f(frame.x, frame.w), vp_rect, VP_invert);
	g_app->GetRayFromScreen(&m_data.m_ray4, alVec2f(frame.z, frame.w), vp_rect, VP_invert);

	m_data.m_ray1.Update();
	m_data.m_ray2.Update();
	m_data.m_ray3.Update();
	m_data.m_ray4.Update();

	m_data.m_top[0] = m_data.m_ray1.m_origin;
	m_data.m_top[1] = m_data.m_ray2.m_origin;
	m_data.m_top[2] = m_data.m_ray2.m_end;
	m_data.m_top[3] = m_data.m_ray1.m_end;

	m_data.m_right[0] = m_data.m_ray2.m_origin;
	m_data.m_right[1] = m_data.m_ray4.m_origin;
	m_data.m_right[2] = m_data.m_ray4.m_end;
	m_data.m_right[3] = m_data.m_ray2.m_end;

	m_data.m_bottom[0] = m_data.m_ray4.m_origin;
	m_data.m_bottom[1] = m_data.m_ray3.m_origin;
	m_data.m_bottom[2] = m_data.m_ray3.m_end;
	m_data.m_bottom[3] = m_data.m_ray4.m_end;

	m_data.m_left[0] = m_data.m_ray3.m_origin;
	m_data.m_left[1] = m_data.m_ray1.m_origin;
	m_data.m_left[2] = m_data.m_ray1.m_end;
	m_data.m_left[3] = m_data.m_ray3.m_end;

	m_data.m_front[0] = m_data.m_ray1.m_origin;
	m_data.m_front[1] = m_data.m_ray2.m_origin;
	m_data.m_front[2] = m_data.m_ray3.m_origin;
	m_data.m_front[3] = m_data.m_ray4.m_origin;

	m_data.m_back[0] = m_data.m_ray1.m_end;
	m_data.m_back[1] = m_data.m_ray2.m_end;
	m_data.m_back[2] = m_data.m_ray3.m_end;
	m_data.m_back[3] = m_data.m_ray4.m_end;

	alVec4 e1, e2;

	e1 = m_data.m_right[1] - m_data.m_right[3];
	e2 = m_data.m_right[2] - m_data.m_right[3];
	e1.Cross2(e2, m_data.m_RN);
	m_data.m_RC = m_data.m_right[0] + m_data.m_right[1] + m_data.m_right[2] + m_data.m_right[3];
	m_data.m_RC *= 0.25;

	e1 = m_data.m_bottom[1] - m_data.m_bottom[3];
	e2 = m_data.m_bottom[2] - m_data.m_bottom[3];
	e1.Cross2(e2, m_data.m_BN);
	m_data.m_BC = m_data.m_bottom[0] + m_data.m_bottom[1] + m_data.m_bottom[2] + m_data.m_bottom[3];
	m_data.m_BC *= 0.25;

	e1 = m_data.m_top[1] - m_data.m_top[3];
	e2 = m_data.m_top[2] - m_data.m_top[3];
	e1.Cross2(e2, m_data.m_TN);
	m_data.m_TC = m_data.m_top[0] + m_data.m_top[1] + m_data.m_top[2] + m_data.m_top[3];
	m_data.m_TC *= 0.25;

	e1 = m_data.m_left[1] - m_data.m_left[3];
	e2 = m_data.m_left[2] - m_data.m_left[3];
	e1.Cross2(e2, m_data.m_LN);
	m_data.m_LC = m_data.m_left[0] + m_data.m_left[1] + m_data.m_left[2] + m_data.m_left[3];
	m_data.m_LC *= 0.25;

	e1 = m_data.m_front[1] - m_data.m_front[0];
	e2 = m_data.m_front[2] - m_data.m_front[0];
	e1.Cross2(e2, m_data.m_FrontN);
	m_data.m_FrontC = m_data.m_front[0] + m_data.m_front[1] + m_data.m_front[2] + m_data.m_front[3];
	m_data.m_FrontC *= 0.25;

	e1 = m_data.m_back[1] - m_data.m_back[0];
	e2 = m_data.m_back[2] - m_data.m_back[0];
	e1.Cross2(e2, m_data.m_BackN);
	m_data.m_BackC = m_data.m_back[0] + m_data.m_back[1] + m_data.m_back[2] + m_data.m_back[3];
	m_data.m_BackC *= 0.25;

	m_data.m_ray5.m_origin = m_data.m_BackC;
	m_data.m_ray5.m_end = m_data.m_FrontC;
	m_data.m_ray5.Update();

	m_data.m_RN.Normalize2();
	m_data.m_LN.Normalize2();
	m_data.m_TN.Normalize2();
	m_data.m_BN.Normalize2();
	m_data.m_BackN.Normalize2();
	m_data.m_FrontN.Normalize2();
}

bool AppSelectionFrust::PointInFrust(const alVec4& v)
{
	if (m_data.m_TN.Dot(m_data.m_TC - v) < 0.f) return false;
	if (m_data.m_BN.Dot(m_data.m_BC - v) < 0.f) return false;
	if (m_data.m_RN.Dot(m_data.m_RC - v) < 0.f) return false;
	if (m_data.m_LN.Dot(m_data.m_LC - v) < 0.f) return false;
	return true;
}

bool AppSelectionFrust::LineInFrust(const alVec4& p1, const alVec4& p2)
{
	// обе точки за пределами
	if (m_data.m_TN.Dot(m_data.m_TC - p1) < 0.f && m_data.m_TN.Dot(m_data.m_TC - p2) < 0.f) return false;
	if (m_data.m_BN.Dot(m_data.m_BC - p1) < 0.f && m_data.m_BN.Dot(m_data.m_BC - p2) < 0.f) return false;
	if (m_data.m_RN.Dot(m_data.m_RC - p1) < 0.f && m_data.m_RN.Dot(m_data.m_RC - p2) < 0.f) return false;
	if (m_data.m_LN.Dot(m_data.m_LC - p1) < 0.f && m_data.m_LN.Dot(m_data.m_LC - p2) < 0.f) return false;

	// обе точки внутри
	if (m_data.m_TN.Dot(m_data.m_TC - p1) > 0.f && m_data.m_TN.Dot(m_data.m_TC - p2) > 0.f)
	{
		if (m_data.m_RN.Dot(m_data.m_RC - p1) > 0.f && m_data.m_RN.Dot(m_data.m_RC - p2) > 0.f)
		{
			if (m_data.m_BN.Dot(m_data.m_BC - p1) > 0.f && m_data.m_BN.Dot(m_data.m_BC - p2) > 0.f)
			{
				if (m_data.m_LN.Dot(m_data.m_LC - p1) > 0.f && m_data.m_LN.Dot(m_data.m_LC - p2) > 0.f)
				{
					return true;
				}
			}
		}
	}

	// try ray-triangle
	if (RayTest_MT(p1, p2, m_data.m_top[0], m_data.m_top[1], m_data.m_top[2])) return true;
	if (RayTest_MT(p1, p2, m_data.m_top[0], m_data.m_top[2], m_data.m_top[3])) return true;
	if (RayTest_MT(p1, p2, m_data.m_left[0], m_data.m_left[1], m_data.m_left[2])) return true;
	if (RayTest_MT(p1, p2, m_data.m_left[0], m_data.m_left[2], m_data.m_left[3])) return true;
	if (RayTest_MT(p1, p2, m_data.m_right[0], m_data.m_right[1], m_data.m_right[2])) return true;
	if (RayTest_MT(p1, p2, m_data.m_right[0], m_data.m_right[2], m_data.m_right[3])) return true;
	if (RayTest_MT(p1, p2, m_data.m_bottom[0], m_data.m_bottom[1], m_data.m_bottom[2])) return true;
	if (RayTest_MT(p1, p2, m_data.m_bottom[0], m_data.m_bottom[2], m_data.m_bottom[3])) return true;

	return false;
}

// using in LineInFrust
bool AppSelectionFrust::RayTest_MT(const alVec4& ray_origin, const alVec4& ray_end, const alVec4& v1, const alVec4& v2, const alVec4& v3)
{
	alVec4 e1 = v2 - v1;
	alVec4 e2 = v3 - v1;
	alVec4 ray_dir = ray_end - ray_origin;
	ray_dir.Normalize2();
	ray_dir.w = 1.f;
	alVec4  pvec;
	ray_dir.Cross2(e2, pvec);
	float det = e1.Dot(pvec);

	if (std::fabs(det) < alEpsilon) return false;

	alVec4 tvec(
		ray_origin.x - v1.x,
		ray_origin.y - v1.y,
		ray_origin.z - v1.z,
		0.f);

	float32_t inv_det = 1.f / det;
	float32_t U = tvec.Dot(pvec) * inv_det;

	if (U < 0.f || U > 1.f)
		return false;

	alVec4  qvec;
	tvec.Cross2(e1, qvec);
	float32_t V = ray_dir.Dot(qvec) * inv_det;

	if (V < 0.f || U + V > 1.f)
		return false;

	//float32_t T = e2.Dot(qvec) * inv_det;
	//if( T < miEpsilon ) return false;
	return true;
}

