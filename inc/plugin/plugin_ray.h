#pragma once
#ifndef _PLUGINRAY_H_
#define _PLUGINRAY_H_


class PluginRay
{
	int max_dim(const plVec4& v)
	{
		return (v.x > v.y) ? ((v.x > v.z)
			? 0 : 2) : ((v.y > v.z) ? 1 : 2);
	}
public:
	PluginRay() {}
	~PluginRay() {}

	plVec4 m_origin;
	plVec4 m_end;
	plVec4 m_direction;
	plVec4 m_invDir;

	int32_t m_kz = 0;
	int32_t m_kx = 0;
	int32_t m_ky = 0;

	float32_t m_Sx = 0.f;
	float32_t m_Sy = 0.f;
	float32_t m_Sz = 0.f;

	void CreateFrom2DCoords(const plVec2i& coord, const plVec4i& rc, const plVec2i& rc_sz, const plMat4& VPinv)
	{
		plVec2i point;
		point.x = coord.x - rc.x;
		point.y = coord.y - rc.y;

		// координаты курсора от -1 до +1
		float pt_x = (point.x / rc_sz.x) * 2.f - 1.f;
		float pt_y = -(point.y / rc_sz.y) * 2.f + 1.f;

		//                                           0.f - for d3d
		m_origin = PluginMath::Mul(plVec4(pt_x, pt_y, -1.f, 1.f), VPinv);
		m_end = PluginMath::Mul(plVec4(pt_x, pt_y, 1.f, 1.f), VPinv);

		m_origin.w = 1.0f / m_origin.w;
		m_origin.x *= m_origin.w;
		m_origin.y *= m_origin.w;
		m_origin.z *= m_origin.w;

		m_end.w = 1.0f / m_end.w;
		m_end.x *= m_end.w;
		m_end.y *= m_end.w;
		m_end.z *= m_end.w;

		Update();
	}

	float32_t DistanceToLine(const plVec4& lineP0, const plVec4& lineP1)
	{
		plVec4 u = m_end - m_origin;
		plVec4 v = lineP1 - lineP0;
		plVec4 w = m_origin - lineP0;
		u.w = 0.f;
		v.w = 0.f;
		w.w = 0.f;
		float32_t a = (float32_t)u.Dot();
		float32_t b = (float32_t)u.Dot(v);
		float32_t c = (float32_t)v.Dot();
		float32_t d = (float32_t)u.Dot(w);
		float32_t e = (float32_t)v.Dot(w);
		float32_t D = a * c - b * b;
		float32_t sc, tc;

		if (D < plEpsilon)
		{
			sc = 0.f;
			tc = (b > c ? d / b : e / c);
		}
		else
		{
			sc = (b * e - c * d) / D;
			tc = (a * e - b * d) / D;
		}

		plVec4 dP = w + (sc * u) - (tc * v);
		dP.w = 0.f;
		return (float32_t)std::sqrt(dP.Dot());
	}

	void Update()
	{
		m_direction.x = m_end.x - m_origin.x;
		m_direction.y = m_end.y - m_origin.y;
		m_direction.z = m_end.z - m_origin.z;
		m_direction.w = 0.f;
		m_direction.Normalize2();

		m_invDir.x = 1.f / m_direction.x;
		m_invDir.y = 1.f / m_direction.y;
		m_invDir.z = 1.f / m_direction.z;
		m_invDir.w = 1.f / m_direction.w;

		m_kz = max_dim
		(
			plVec4
			(
				std::abs(m_direction.x),
				std::abs(m_direction.y),
				std::abs(m_direction.z),
				1.f
			)
		);

		m_kx = m_kz + 1;
		if (m_kx == 3)
			m_kx = 0;

		m_ky = m_kx + 1;
		if (m_ky == 3)
			m_ky = 0;

		auto dir_data = m_direction.Data();
		if (dir_data[m_kz])
			std::swap(m_kx, m_ky);

		m_Sx = float32_t(dir_data[m_kx] / dir_data[m_kz]);
		m_Sy = float32_t(dir_data[m_ky] / dir_data[m_kz]);
		m_Sz = float32_t(1.0 / dir_data[m_kz]);
	}
	void GetIntersectionPoint(int32_t t, plVec4& ip)
	{
		ip = m_origin + (float32_t)t * m_direction;
	}
	void GetIntersectionPoint(float32_t t, plVec4& ip)
	{
		ip = m_origin + t * m_direction;
	}
	void GetIntersectionPoint(float64_t t, plVec4& ip)
	{
		ip = m_origin + t * m_direction;
	}
	bool PlaneIntersection(const plVec4& planePoint, const plVec4f& planeNormal, float64_t& T)
	{
		float64_t det = (planeNormal.x * m_direction.x) + (planeNormal.y * m_direction.y) + (planeNormal.z * m_direction.z);

		if (std::fabs(det) < plEpsilon)
			return false;

		plVec4 v;
		v.x = planePoint.x - m_origin.x;
		v.y = planePoint.y - m_origin.y;
		v.z = planePoint.z - m_origin.z;

		T = (planeNormal.x * v.x) + (planeNormal.y * v.y) + (planeNormal.z * v.z);

		T /= det;
		return true;
	}



};

#endif