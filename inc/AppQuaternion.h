#pragma once
#ifndef _ELQ_H_
#define _ELQ_H_

class AppQuaternion
{
public:

	float32_t x = 0;
	float32_t y = 0;
	float32_t z = 0;
	float32_t w = 0;

	AppQuaternion()
	{
		Identity();
	}

	AppQuaternion(const AppQuaternion& o) :
		x(o.x),
		y(o.y),
		z(o.z),
		w(o.w)
	{
	}

	AppQuaternion(float32_t i1, float32_t i2, float32_t i3, float32_t real)
		:
		x(i1),
		y(i2),
		z(i3),
		w(real)
	{
	}

	AppQuaternion(const AppVec3f& _axis, const float32_t& _angle)
	{
		SetRotation(_axis, _angle);
	}

	AppQuaternion(float32_t Euler_x, float32_t Euler_y, float32_t Euler_z)
	{
		Set(Euler_x, Euler_y, Euler_z);
	}

	AppQuaternion(const AppVec4f& v) { Set(v); }

	float32_t* Data() { return &x; }
	AppVec4f	Get() const { return AppVec4f(x, y, z, w); }
	float32_t	GetReal() const { return w; }

	void Set(float32_t i1, float32_t i2, float32_t i3, float32_t real)
	{
		x = i1;
		y = i2;
		z = i3;
		w = real;
	}

	void Set(const AppVec4f& v) { Set(v.x, v.y, v.z); }

	void Set(float32_t Euler_x, float32_t Euler_y, float32_t Euler_z)
	{
		Euler_x *= 0.5f;
		Euler_y *= 0.5f;
		Euler_z *= 0.5f;
		float32_t c1 = cos(Euler_x);
		float32_t c2 = cos(Euler_y);
		float32_t c3 = cos(Euler_z);
		float32_t s1 = sin(Euler_x);
		float32_t s2 = sin(Euler_y);
		float32_t s3 = sin(Euler_z);
		w = (c1 * c2 * c3) + (s1 * s2 * s3);
		x = (s1 * c2 * c3) - (c1 * s2 * s3);
		y = (c1 * s2 * c3) + (s1 * c2 * s3);
		z = (c1 * c2 * s3) - (s1 * s2 * c3);
	}

	void SetRotation(const AppVec3f& axis, const float32_t& _angle)
	{
		float32_t d = axis.Length();
		float32_t s = sin(_angle * 0.5f) / d;
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
		w = cos(_angle * 0.5f);
	}

	void SetReal(float32_t r)
	{
		w = r;
	}

	void Identity()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 1.f;
	}

	AppQuaternion& operator=(const AppQuaternion& o)
	{
		x = o.x;
		y = o.y;
		z = o.z;
		w = o.w;
		return *this;
	}

	AppQuaternion operator*(const AppQuaternion& q)const
	{
		return AppQuaternion(
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y + y * q.w + z * q.x - x * q.z,
			w * q.z + z * q.w + x * q.y - y * q.x,
			w * q.w - x * q.x - y * q.y - z * q.z);
	}

	void operator*=(const AppQuaternion& q)
	{
		x = w * q.x + x * q.w + y * q.z - z * q.y;
		y = w * q.y + y * q.w + z * q.x - x * q.z;
		z = w * q.z + z * q.w + x * q.y - y * q.x;
		w = w * q.w - x * q.x - y * q.y - z * q.z;
	}

	bool operator!=(const AppQuaternion& q)const
	{
		if (x != q.x) return true;
		if (y != q.y) return true;
		if (z != q.z) return true;
		if (w != q.w) return true;
		return false;
	}

	bool operator==(const AppQuaternion& q)const
	{
		if (x != q.x) return false;
		if (y != q.y) return false;
		if (z != q.z) return false;
		if (w != q.w) return false;
		return true;
	}

	AppQuaternion operator+(const AppQuaternion& o) const
	{
		return AppQuaternion(
			x + o.x,
			y + o.y,
			z + o.z,
			w + o.w);
	}

	AppQuaternion operator-(const AppQuaternion& o) const
	{
		return AppQuaternion(
			x - o.x,
			y - o.y,
			z - o.z,
			w - o.w);
	}

	AppQuaternion operator-()
	{
		x = -x;
		y = -y;
		z = -z;
		return AppQuaternion(x, y, z, w);
	}

	float32_t operator[](uint32_t index) const
	{
		return (&x)[index];
	}

	float32_t& operator[](uint32_t index)
	{
		return (&x)[index];
	}

	float32_t GetLength() { return std::sqrt(x * x + y * y + z * z + w * w); }
	float32_t Length2() const { return Dot(*this); }

	AppQuaternion operator*(float32_t s) const
	{
		return AppQuaternion(s * x, s * y, s * z, s * w);
	}

	AppQuaternion Lerp(AppQuaternion q1, AppQuaternion q2, float32_t time)
	{
		const float32_t scale = 1.0f - time;
		return (*this = (q1 * scale) + (q2 * time));
	}

	float32_t Dot(const AppQuaternion& q2) const
	{
		return (x * q2.x) + (y * q2.y) + (z * q2.z) + (w * q2.w);
	}

	AppQuaternion Slerp(AppQuaternion q1, AppQuaternion q2, float32_t time, float32_t threshold)
	{
		float32_t angle = q1.Dot(q2);
		// make sure we use the short rotation
		if (angle < 0.0f)
		{
			q1 = q1 * -1.0f;
			angle *= -1.0f;
		}

		if (angle <= (1 - threshold)) // spherical interpolation
		{
			const float32_t theta = acosf(angle);
			const float32_t invsintheta = 1.f / (sinf(theta));
			const float32_t scale = sinf(theta * (1.0f - time)) * invsintheta;
			const float32_t invscale = sinf(theta * time) * invsintheta;
			return (*this = (q1 * scale) + (q2 * invscale));
		}
		else // linear interploation
			return Lerp(q1, q2, time);
	}

	AppQuaternion& Normalize()
	{
		float32_t len = this->GetLength();
		if (len) {
			float32_t len2 = 1.f / len;
			x *= len2;
			y *= len2;
			z *= len2;
			w *= len2;
		}
		return *this;
	}

	void Invert()
	{
		x = -x;
		y = -y;
		z = -z;
	}

};


#endif