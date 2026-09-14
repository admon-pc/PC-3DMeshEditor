#pragma once
#ifndef _APPAABB_H_
#define _APPAABB_H_


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// Axis-Aligned Bounding Box
class AppAabb
{
public:

	AppAabb()
		:
		m_min(AppVec4FltMax),
		m_max(AppVec4FltMaxNeg)
	{}

	AppAabb(const AppVec4& min, const AppVec4& max) : m_min(min), m_max(max) { }
	AppAabb(const AppVec4f& min, const AppVec4f& max)
	{
		m_min.x = min.x;
		m_min.y = min.y;
		m_min.z = min.z;
	}

	/*
	Transforming Axis-Aligned Bounding Boxes
	by Jim Arvo
	from "Graphics Gems", Academic Press, 1990
	*/
	void Transform(AppAabb* original, AppMat4* matrix, AppVec4* position)
	{
		float64_t  a, b;
		float64_t  Amin[3], Amax[3];
		float64_t  Bmin[3], Bmax[3];
		int32_t    i, j;

		/*Copy box A into a min array and a max array for easy reference.*/
		Amin[0] = original->m_min.x;  Amax[0] = original->m_max.x;
		Amin[1] = original->m_min.y;  Amax[1] = original->m_max.y;
		Amin[2] = original->m_min.z;  Amax[2] = original->m_max.z;

		/* Take care of translation by beginning at T. */
		Bmin[0] = Bmax[0] = position->x;
		Bmin[1] = Bmax[1] = position->y;
		Bmin[2] = Bmax[2] = position->z;

		/* Now find the extreme points by considering the product of the */
		/* min and max with each component of M.  */
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				// mimi: I swapped j and i
				a = (float32_t)(matrix->m_data[j][i] * Amin[j]);
				b = (float32_t)(matrix->m_data[j][i] * Amax[j]);
				if (a < b)
				{
					Bmin[i] += a;
					Bmax[i] += b;
				}
				else
				{
					Bmin[i] += b;
					Bmax[i] += a;
				}
			}
		}
		/* Copy the result into the new box. */
		m_min.x = Bmin[0];  m_max.x = Bmax[0];
		m_min.y = Bmin[1];  m_max.y = Bmax[1];
		m_min.z = Bmin[2];  m_max.z = Bmax[2];
	}

	void Add(const AppVec4& point)
	{
		if (point.x < m_min.x) m_min.x = point.x;
		if (point.y < m_min.y) m_min.y = point.y;
		if (point.z < m_min.z) m_min.z = point.z;

		if (point.x > m_max.x) m_max.x = point.x;
		if (point.y > m_max.y) m_max.y = point.y;
		if (point.z > m_max.z) m_max.z = point.z;
	}
	void Add(const AppVec4f& point)
	{
		if (point.x < m_min.x) m_min.x = point.x;
		if (point.y < m_min.y) m_min.y = point.y;
		if (point.z < m_min.z) m_min.z = point.z;

		if (point.x > m_max.x) m_max.x = point.x;
		if (point.y > m_max.y) m_max.y = point.y;
		if (point.z > m_max.z) m_max.z = point.z;
	}
	void Add(const AppVec3f& point)
	{
		if (point.x < m_min.x) m_min.x = point.x;
		if (point.y < m_min.y) m_min.y = point.y;
		if (point.z < m_min.z) m_min.z = point.z;

		if (point.x > m_max.x) m_max.x = point.x;
		if (point.y > m_max.y) m_max.y = point.y;
		if (point.z > m_max.z) m_max.z = point.z;
	}

	void Add(const AppAabb& box)
	{
		if (box.m_min.x < m_min.x) m_min.x = box.m_min.x;
		if (box.m_min.y < m_min.y) m_min.y = box.m_min.y;
		if (box.m_min.z < m_min.z) m_min.z = box.m_min.z;

		if (box.m_max.x > m_max.x) m_max.x = box.m_max.x;
		if (box.m_max.y > m_max.y) m_max.y = box.m_max.y;
		if (box.m_max.z > m_max.z) m_max.z = box.m_max.z;
	}

	bool RayTest(const AppRay& r)
	{
		float64_t t1 = (m_min.x - r.m_origin.x) * r.m_invDir.x;
		float64_t t2 = (m_max.x - r.m_origin.x) * r.m_invDir.x;
		float64_t t3 = (m_min.y - r.m_origin.y) * r.m_invDir.y;
		float64_t t4 = (m_max.y - r.m_origin.y) * r.m_invDir.y;
		float64_t t5 = (m_min.z - r.m_origin.z) * r.m_invDir.z;
		float64_t t6 = (m_max.z - r.m_origin.z) * r.m_invDir.z;

		float64_t tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float64_t tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		if (tmax < 0 || tmin > tmax) return false;

		return true;
	}

	void Center(AppVec4& v) const
	{
		v = AppVec4(m_min + m_max);
		v *= 0.5f;
	}

	float64_t Radius()
	{
		return m_min.Distance(m_max) * 0.5f;
	}

	void Extent(AppVec4& v)
	{
		v = AppVec4(m_max - m_min);
	}

	bool IsEmpty() const
	{
		return ((m_min == AppVec4FltMax) && (m_max == AppVec4FltMaxNeg))
			|| (m_min == m_max);
	}

	void Reset()
	{
		m_min = AppVec4FltMax;
		m_max = AppVec4FltMaxNeg;
	}



	AppVec4 m_min;
	AppVec4 m_max;
};

#endif